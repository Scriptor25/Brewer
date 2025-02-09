#include <utility>
#include <Brewer/Module.hpp>
#include <Brewer/Type.hpp>
#include <Brewer/Printer.hpp>
#include <Brewer/Value/GlobalValue.hpp>

Brewer::Module::Module(Context &context, std::string filename)
    : m_Context(context),
      m_Filename(std::move(filename))
{
}

Brewer::Context &Brewer::Module::GetContext() const
{
    return m_Context;
}

std::string Brewer::Module::GetFilename() const
{
    return m_Filename;
}

void Brewer::Module::Print(ASMPrinterBase *printer)
{
    printer->Print(*this);
}

std::ostream &Brewer::Module::PrintIR(std::ostream &os) const
{
    bool first = true;
    for (const auto &global: m_SymbolTable)
    {
        if (first)
            first = false;
        else
            os << std::endl;
        global->PrintIR(os) << std::endl;
    }
    return os;
}

void Brewer::Module::Append(GlobalValue *value)
{
    if (const auto old_value = Find(m_SymbolTable, value->GetName()))
    {
        Replace(m_SymbolTable, old_value, value);
        old_value->ReplaceWith(value);
        return;
    }

    m_SymbolTable.push_back(value);
}

Brewer::GlobalValue *Brewer::Module::Get(PointerType *type, const std::string &name)
{
    if (const auto global = Find(m_SymbolTable, type, name))
        return global;

    const auto global = new GlobalValue(type->GetElementType(), name, GlobalValue::Linkage_Local, {});
    m_SymbolTable.push_back(global);
    return global;
}

void Brewer::Module::ForEach(const std::function<void(GlobalValue *)> &consumer) const
{
    for (const auto &global: m_SymbolTable)
        consumer(global);
}

void Brewer::Module::ValidateAndOptimize()
{
    // remove empty (unused!) blocks
    for (const auto &global: m_SymbolTable)
        if (const auto fun = dynamic_cast<GlobalFunction *>(global))
            for (unsigned i = 0; i < fun->GetNumBlocks();)
                if (fun->GetBlock(i)->IsEmpty() && fun->GetBlock(i)->GetNumUses() == 0)
                    fun->EraseBlock(i);
                else
                    ++i;

    // remove unused global variables (local linkage only)
    for (auto it = m_SymbolTable.begin(); it != m_SymbolTable.end(); ++it)
        if (const auto var = dynamic_cast<GlobalVariable *>(*it))
            if (var->GetLinkage() == GlobalValue::Linkage_Local && var->GetNumUses() == 0)
                m_SymbolTable.erase(it--);

    // remove unused global functions (local linkage or totally extern)
    for (auto it = m_SymbolTable.begin(); it != m_SymbolTable.end(); ++it)
        if (const auto fun = dynamic_cast<GlobalFunction *>(*it))
            if ((fun->GetLinkage() == GlobalValue::Linkage_Local || fun->IsEmpty()) && fun->GetNumUses() == 0)
                m_SymbolTable.erase(it--);

    // validate that there are no unresolved blocks
    for (const auto &global: m_SymbolTable)
        if (const auto fun = dynamic_cast<GlobalFunction *>(global))
            if (fun->HasUnresolved())
                Error("function has unresolved blocks: {}", fun);

    // validate that every block has a terminator (br | ret)
    for (const auto &global: m_SymbolTable)
        if (const auto fun = dynamic_cast<GlobalFunction *>(global))
            for (unsigned i = 0; i < fun->GetNumBlocks(); ++i)
            {
                const auto block = fun->GetBlock(i);
                if (!block->IsEmpty() && block->GetValue(block->GetNumValues() - 1)->IsTerminator())
                    continue;
                Error("block is missing a terminator instruction like br or ret: {}", block);
            }

    // remove unused assignments
    for (const auto &global: m_SymbolTable)
        if (const auto fun = dynamic_cast<GlobalFunction *>(global))
            for (unsigned i = 0; i < fun->GetNumBlocks(); ++i)
            {
                const auto block = fun->GetBlock(i);
                for (unsigned j = 0; j < block->GetNumValues(); ++j)
                    if (const auto assign = dynamic_cast<Assignment *>(block->GetValue(j)))
                        if (assign->GetDst()->GetNumUses() == 0)
                        {
                            block->Replace(assign, assign->GetSrc());
                            assign->GetSrc()->RemoveUse(assign);
                        }
            }

    // remove ineffective values (like standalone add instruction)
    for (const auto &global: m_SymbolTable)
        if (const auto fun = dynamic_cast<GlobalFunction *>(global))
            for (unsigned i = 0; i < fun->GetNumBlocks(); ++i)
            {
                const auto block = fun->GetBlock(i);
                for (unsigned j = 0; j < block->GetNumValues(); ++j)
                    if (const auto value = block->GetValue(j); value->GetNumUses() == 0 && value->RequiresDestination())
                    {
                        block->Erase(value);
                        --j;
                    }
            }

    /* TODO:
     * if two blocks A and B are directly adjacent, aka the terminator from A points to B, and B only has the one entry
     * point coming from A, so only one reference/use, the blocks can be merged into one unified block AB, replacing the
     * terminator of A.
     */
}

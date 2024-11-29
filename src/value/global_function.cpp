#include <Brewer/Context.hpp>
#include <Brewer/Type.hpp>
#include <Brewer/Value/GlobalValue.hpp>

Brewer::GlobalFunction::GlobalFunction(
    FunctionType* type,
    std::string name,
    const Linkage linkage,
    std::vector<FunctionArg*> args)
    : GlobalValue(type, std::move(name), linkage), m_Args(std::move(args))
{
}

Brewer::FunctionArg* Brewer::GlobalFunction::GetArg(const unsigned i) const
{
    return m_Args[i];
}

unsigned Brewer::GlobalFunction::GetNumArgs() const
{
    return m_Args.size();
}

Brewer::FunctionBlock* Brewer::GlobalFunction::GetBlock(const unsigned i) const
{
    return m_Blocks[i];
}

unsigned Brewer::GlobalFunction::GetNumBlocks() const
{
    return m_Blocks.size();
}

bool Brewer::GlobalFunction::IsEmpty() const
{
    return GetNumBlocks() == 0;
}

bool Brewer::GlobalFunction::HasUnresolved() const
{
    return !m_Unresolved.empty();
}

void Brewer::GlobalFunction::Append(Value* value)
{
    if (const auto block = dynamic_cast<FunctionBlock*>(value))
    {
        if (const auto old_block = Erase(m_Unresolved, block->GetName()))
            old_block->ReplaceWith(block);
        if (m_Blocks.empty()) block->AddUse(this);
        m_Blocks.push_back(block);
        return;
    }

    if (m_Blocks.empty())
    {
        const auto block = new FunctionBlock(GetType()->GetContext().GetBlockType(), {});
        block->AddUse(this);
        m_Blocks.emplace_back() = block;
    }

    m_Blocks.back()->Append(value);
}

void Brewer::GlobalFunction::EraseBlock(const unsigned i)
{
    m_Blocks.erase(m_Blocks.begin() + i);
}

Brewer::NamedValue* Brewer::GlobalFunction::Get(Type* type, const std::string& name)
{
    if (const auto block_type = dynamic_cast<BlockType*>(type))
    {
        if (const auto block = Find(m_Blocks, type, name))
            return block;
        return m_Unresolved.emplace_back() = new FunctionBlock(block_type, name);
    }

    if (const auto arg = Find(m_Args, type, name))
        return arg;

    if (const auto local = Find(m_Locals, type, name))
        return local;

    const auto local = new NamedValue(type, name);
    return m_Locals.emplace_back() = local;
}

uint64_t Brewer::GlobalFunction::GetNumAllocaBytes() const
{
    uint64_t bytes = 0;
    uint64_t max_shadow = 0;
    for (const auto local : m_Locals)
        bytes += local->GetType()->GetNumBytes();
    for (const auto block : m_Blocks)
        for (unsigned i = 0; i < block->GetNumValues(); ++i)
        {
            const auto value = block->GetValue(i);
            const auto b = value->GetNumAllocaBytes();
            if (const auto inst = dynamic_cast<Instruction*>(value);
                inst && inst->GetCode() == Instruction::Call && b > max_shadow)
                max_shadow = b;
            else bytes += b;
        }
    return bytes;
}

std::ostream& Brewer::GlobalFunction::PrintIR(std::ostream& os) const
{
    os << "fun ";
    GetType()->GetElementType()->GetResultType()->Print(os);
    os << ' ';
    if (GetLinkage() != Linkage_Global)
        os << ToString(GetLinkage()) << ' ';
    os << '@' << GetName() << '(';
    for (unsigned i = 0; i < m_Args.size(); ++i)
    {
        if (i > 0) os << ", ";
        m_Args[i]->PrintIR(os);
    }
    if (GetType()->GetElementType()->IsVarArg())
    {
        if (!m_Args.empty()) os << ", ";
        os << "...";
    }
    os << ')';
    if (IsEmpty()) return os;

    os << " {" << std::endl;
    for (const auto block : m_Blocks)
        block->PrintIR(os);
    return os << '}';
}

#include <utility>
#include <Brewer/Context.hpp>
#include <Brewer/Type.hpp>
#include <Brewer/Value/Argument.hpp>
#include <Brewer/Value/Block.hpp>
#include <Brewer/Value/Function.hpp>

Brewer::Function::Function(
    FunctionType* function_type,
    std::string name,
    const LinkageType linkage,
    std::vector<Argument*> args)
    : GlobalValue(function_type, std::move(name), linkage), m_Args(std::move(args))
{
    for (const auto& arg : m_Args)
        arg->AddUse(this);
}

std::ostream& Brewer::Function::PrintIR(std::ostream& os) const
{
    const auto type = dynamic_cast<FunctionType*>(dynamic_cast<PointerType*>(GetType())->ElementType());

    if (m_Blocks.empty())
        os << "declare ";
    else os << "define ";

    type->ResultType()->Print(os << GetLinkage() << ' ') << " @" << GetName() << '(';
    for (unsigned i = 0; i < m_Args.size(); ++i)
    {
        if (i > 0) os << ", ";
        m_Args[i]->PrintIROperand(os);
    }
    if (type->VarArg())
    {
        if (!m_Args.empty())
            os << ", ";
        os << "...";
    }
    os << ')';

    if (m_Blocks.empty())
        return os;

    os << " {" << std::endl;
    for (const auto& block : m_Blocks)
        block->PrintIR(os);
    return os << '}';
}

void Brewer::Function::ReplaceUseOf(Value* old_value, Value* new_value)
{
    Replace(m_Args, old_value, new_value);
    Replace(m_Blocks, old_value, new_value);
}

Brewer::NamedValue* Brewer::Function::GetValue(Type* type, const std::string& name)
{
    if (const auto value = Find(m_PreUses, type, name))
        return value;
    if (const auto value = Find(m_Args, type, name))
        return value;
    if (const auto value = Find(m_Blocks, type, name))
        return value;

    for (const auto& block : m_Blocks)
        if (const auto value = block->GetValue(type, name))
            return value;

    const auto pre_use = new NamedValue(type, name);
    m_PreUses.push_back(pre_use);
    return pre_use;
}

void Brewer::Function::Append(Value* value)
{
    if (const auto named = dynamic_cast<NamedValue*>(value))
    {
        if (const auto pre_use = Erase(m_PreUses, named->GetName()))
            pre_use->ReplaceAllUses(value);

        if (const auto block = dynamic_cast<Block*>(value))
        {
            block->AddUse(this);
            m_Blocks.push_back(block);
            return;
        }
    }

    if (m_Blocks.empty())
    {
        const auto block = new Block(value->GetType()->GetContext().GetBlockType(), {});
        block->AddUse(this);
        m_Blocks.push_back(block);
    }

    m_Blocks.back()->Append(value);
}

bool Brewer::Function::IsEmpty() const
{
    return m_Blocks.empty();
}

Brewer::Argument* Brewer::Function::GetArgument(const unsigned i) const
{
    return m_Args[i];
}

unsigned Brewer::Function::GetNumArguments() const
{
    return m_Args.size();
}

Brewer::Block* Brewer::Function::GetBlock(const unsigned i) const
{
    return m_Blocks[i];
}

unsigned Brewer::Function::GetNumBlocks() const
{
    return m_Blocks.size();
}

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

std::ostream& Brewer::Function::Print(std::ostream& os) const
{
    const auto type = dynamic_cast<FunctionType*>(dynamic_cast<PointerType*>(GetType())->ElementType());

    if (m_Blocks.empty()) os << "declare ";
    else os << "define ";

    type->ResultType()->Print(os << GetLinkage() << ' ') << " @" << GetName() << '(';
    for (unsigned i = 0; i < m_Args.size(); ++i)
    {
        if (i > 0) os << ", ";
        m_Args[i]->PrintOperand(os);
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
        block->Print(os);
    return os << '}';
}

void Brewer::Function::ReplaceUseOf(Value* old_value, Value* new_value)
{
    for (auto& arg : m_Args)
        if (arg == old_value)
        {
            arg = dynamic_cast<Argument*>(new_value);
            return;
        }
    for (auto& block : m_Blocks)
        if (block == old_value)
        {
            block = dynamic_cast<Block*>(new_value);
            return;
        }
    Error("replace invalid use");
}

Brewer::Block* Brewer::Function::GetBlock(BlockType* type, const std::string& name)
{
    for (const auto& m_Block : m_Blocks)
        if (m_Block->GetName() == name) return m_Block;

    const auto block = new Block(type, name);
    block->AddUse(this);
    m_Blocks.push_back(block);
    return block;
}

Brewer::NamedValue* Brewer::Function::GetValue(Type* type, const std::string& name)
{
    for (const auto& arg : m_Args)
        if (arg->GetName() == name)
        {
            if (arg->GetType() != type)
                Error("type mismatch: {} != {}", arg->GetType(), type);
            return arg;
        }
    for (const auto& block : m_Blocks)
        if (const auto value = block->GetValue(type, name))
            return value;

    const auto named = new NamedValue(type, name);
    m_PreUses.push_back(named);
    return named;
}

void Brewer::Function::Append(Value* value)
{
    if (const auto block = dynamic_cast<Block*>(value))
    {
        unsigned i = 0;
        for (; i < m_Blocks.size(); ++i)
            if (m_Blocks[i]->GetName() == block->GetName())
            {
                m_Blocks[i]->ReplaceAllUses(block);
                break;
            }
        if (i >= m_Blocks.size())
        {
            block->AddUse(this);
            m_Blocks.push_back(block);
        }
        m_InsertBlock = block;
        return;
    }

    if (!m_InsertBlock)
    {
        m_InsertBlock = new Block(value->GetType()->GetContext().GetBlockType(), {});
        m_InsertBlock->AddUse(this);
        m_Blocks.push_back(m_InsertBlock);
    }

    if (const auto named = dynamic_cast<NamedValue*>(value))
        for (unsigned i = 0; i < m_PreUses.size(); ++i)
            if (named->GetName() == m_PreUses[i]->GetName())
            {
                m_PreUses[i]->ReplaceAllUses(value);
                m_PreUses.erase(m_PreUses.begin() + i);
                break;
            }

    m_InsertBlock->Append(value);
}

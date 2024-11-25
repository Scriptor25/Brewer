#include <Brewer/Type.hpp>
#include <Brewer/Value/GlobalValue.hpp>

#include "Brewer/Context.hpp"

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

unsigned Brewer::GlobalFunction::GetByteAlloc() const
{
    return 0;
}

void Brewer::GlobalFunction::Append(Value* value)
{
    if (const auto block = dynamic_cast<FunctionBlock*>(value))
    {
        m_Blocks.push_back(block);
        return;
    }

    if (m_Blocks.empty())
    {
        m_Blocks.emplace_back() = new FunctionBlock(value->GetType()->GetContext().GetBlockType(), {});
        m_Blocks.back()->AddUse(this);
    }

    m_Blocks.back()->Append(value);
}

Brewer::NamedValue* Brewer::GlobalFunction::Get(Type* type, const std::string& name)
{
    if (const auto block_type = dynamic_cast<BlockType*>(type))
    {
        for (const auto& block : m_Blocks)
            if (block->GetName() == name) return block;
        return m_Unresolved.emplace_back() = new FunctionBlock(block_type, name);
    }

    for (const auto& block : m_Blocks)
        if (const auto value = block->Get(type, name))
            return value;

    return m_Unresolved.emplace_back() = new NamedValue(type, name);
}

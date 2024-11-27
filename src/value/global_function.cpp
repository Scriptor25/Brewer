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
    for (const auto& arg : m_Args)
        arg->AddUse(this);
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
    unsigned bytes = 0;
    for (const auto local : m_Locals)
        bytes += local->GetType()->CountBytes();
    return bytes;
}

void Brewer::GlobalFunction::Append(Value* value)
{
    if (const auto block = dynamic_cast<FunctionBlock*>(value))
    {
        if (const auto old_block = Erase(m_Unresolved, block->GetName()))
            old_block->ReplaceWith(block);
        m_Blocks.push_back(block);
        block->AddUse(this);
        return;
    }

    if (m_Blocks.empty())
        m_Blocks.emplace_back() = new FunctionBlock(GetType()->GetContext().GetBlockType(), {});

    m_Blocks.back()->Append(value);
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
    local->AddUse(this);
    return m_Locals.emplace_back() = local;
}

void Brewer::GlobalFunction::Replace(Value* old_value, Value* new_value)
{
    Brewer::Replace(m_Args, old_value, new_value);
    Brewer::Replace(m_Locals, old_value, new_value);
    Brewer::Replace(m_Blocks, old_value, new_value);
}

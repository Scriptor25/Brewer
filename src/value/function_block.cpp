#include <Brewer/Type.hpp>
#include <Brewer/Value/NamedValue.hpp>

Brewer::FunctionBlock::FunctionBlock(BlockType *type, std::string name, std::vector<std::string> &&meta)
    : NamedValue(type, std::move(name), std::move(meta))
{
}

Brewer::Value *Brewer::FunctionBlock::GetValue(const unsigned i) const
{
    return m_Values[i];
}

unsigned Brewer::FunctionBlock::GetNumValues() const
{
    return m_Values.size();
}

bool Brewer::FunctionBlock::IsEmpty() const
{
    return m_Values.empty();
}

void Brewer::FunctionBlock::Append(Value *value)
{
    m_Values.push_back(value);
}

void Brewer::FunctionBlock::Erase(Value *value)
{
    Brewer::Erase(m_Values, value);
}

Brewer::NamedValue *Brewer::FunctionBlock::Get(Type *type, const std::string &name) const
{
    return Find(m_Values, type, name);
}

void Brewer::FunctionBlock::Replace(Value *old_value, Value *new_value)
{
    Brewer::Replace(m_Values, old_value, new_value);
}

std::ostream &Brewer::FunctionBlock::PrintIR(std::ostream &os) const
{
    if (!GetName().empty())
        os << GetName() << ':' << std::endl;
    for (const auto value: m_Values)
        value->PrintIR(os << "  ") << std::endl;
    return os;
}

std::ostream &Brewer::FunctionBlock::PrintOperandIR(std::ostream &os, bool omit_type) const
{
    return os << '%' << GetName();
}

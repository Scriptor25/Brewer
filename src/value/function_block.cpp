#include <Brewer/Type.hpp>
#include <Brewer/Value/NamedValue.hpp>

Brewer::FunctionBlock::FunctionBlock(BlockType* type, std::string name)
    : NamedValue(type, std::move(name))
{
}

Brewer::Value* Brewer::FunctionBlock::GetValue(const unsigned i) const
{
    return m_Values[i];
}

unsigned Brewer::FunctionBlock::GetNumValues() const
{
    return m_Values.size();
}

void Brewer::FunctionBlock::Append(Value* value)
{
    m_Values.push_back(value);
}

Brewer::NamedValue* Brewer::FunctionBlock::Get(Type* type, const std::string& name) const
{
    return Find(m_Values, type, name);
}

void Brewer::FunctionBlock::Replace(Value* old_value, Value* new_value)
{
    Brewer::Replace(m_Values, old_value, new_value);
}

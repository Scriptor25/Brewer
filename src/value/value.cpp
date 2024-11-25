#include <Brewer/Value/Value.hpp>

static unsigned INDEX = 0;

Brewer::Value::Value(Type* type)
    : m_Index(INDEX++), m_Type(type)
{
}

unsigned Brewer::Value::GetIndex() const
{
    return m_Index;
}

Brewer::Type* Brewer::Value::GetType() const
{
    return m_Type;
}

void Brewer::Value::AddUse(Value* user)
{
    m_UseList.push_back(user);
}

void Brewer::Value::ReplaceWith(Value* new_value)
{
    for (const auto& use : m_UseList)
        use->Replace(this, new_value);
    m_UseList.clear();
    delete this;
}

Brewer::Value::~Value()
{
    --INDEX;
}

void Brewer::Value::Replace(Value* old_value, Value* new_value) const
{
}

std::ostream& Brewer::Value::PrintIR(std::ostream& os) const
{
    return PrintOperandIR(os, false);
}

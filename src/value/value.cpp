#include <utility>
#include <Brewer/Value/Value.hpp>

Brewer::Value::Value(Type* type, std::string name)
    : m_Type(type), m_Name(std::move(name))
{
}

Brewer::Value* Brewer::Value::GetPrevious() const
{
    return m_Previous;
}

Brewer::Value* Brewer::Value::GetNext() const
{
    return m_Next;
}

void Brewer::Value::SetPrevious(Value* value)
{
    m_Previous = value;
}

void Brewer::Value::SetNext(Value* value)
{
    m_Next = value;
}

#include <Brewer/Value/Value.hpp>

Brewer::Value::Value(Type* type)
    : m_Type(type)
{
}

Brewer::Type* Brewer::Value::GetType() const
{
    return m_Type;
}

std::ostream& Brewer::Value::PrintUseList(std::ostream& os) const
{
    if (m_UseList.empty())
        return os << "; no uses";
    os << "; ";
    for (unsigned i = 0; i < m_UseList.size(); ++i)
    {
        if (i > 0) os << ", ";
        m_UseList[i]->PrintOperand(os);
    }
    return os;
}

void Brewer::Value::AddUse(Value* use)
{
    m_UseList.push_back(use);
}

void Brewer::Value::ReplaceAllUses(Value* new_value)
{
    for (const auto& use : m_UseList)
    {
        use->ReplaceUseOf(this, new_value);
        new_value->AddUse(use);
    }
    delete this;
}

void Brewer::Value::ReplaceUseOf(Value* old_value, Value* new_value)
{
    Error("this does not use anything");
}

void Brewer::Value::InsertBefore(Value* value)
{
    if (m_Previous) m_Previous->m_Next = m_Next;
    if (m_Next) m_Next->m_Previous = m_Previous;

    m_Previous = value->m_Previous;
    m_Previous->m_Next = this;
    value->m_Previous = this;
    m_Next = value;
}

void Brewer::Value::InsertAfter(Value* value)
{
    if (m_Previous) m_Previous->m_Next = m_Next;
    if (m_Next) m_Next->m_Previous = m_Previous;

    m_Next = value->m_Next;
    m_Next->m_Previous = this;
    value->m_Next = this;
    m_Previous = value;
}

void Brewer::Value::PrependBefore(Value* value)
{
    Value* ptr = value;
    while (ptr->m_Previous)
        ptr = ptr->m_Previous;
    InsertBefore(ptr);
}

void Brewer::Value::AppendAfter(Value* value)
{
    Value* ptr = value;
    while (ptr->m_Next)
        ptr = ptr->m_Next;
    InsertAfter(ptr);
}

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

#include <iostream>
#include <Brewer/Type.hpp>
#include <Brewer/Value/NamedValue.hpp>

Brewer::NamedValue::NamedValue(Type* type, std::string name)
    : Value(type), m_Name(std::move(name))
{
}

std::ostream& Brewer::NamedValue::Print(std::ostream& os) const
{
    return PrintOperand(os);
}

std::ostream& Brewer::NamedValue::PrintOperand(std::ostream& os) const
{
    return GetType()->Print(os) << " %" << m_Name;
}

const std::string& Brewer::NamedValue::GetName() const
{
    return m_Name;
}

void Brewer::NamedValue::SetName(const std::string& name)
{
    m_Name = name;
}

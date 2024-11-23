#include <iostream>
#include <Brewer/Type.hpp>
#include <Brewer/Value/NamedValue.hpp>

Brewer::NamedValue::NamedValue(Type* type, std::string name)
    : Value(type), m_Name(std::move(name))
{
}

std::ostream& Brewer::NamedValue::PrintIR(std::ostream& os) const
{
    return PrintIROperand(os);
}

std::ostream& Brewer::NamedValue::PrintIROperand(std::ostream& os) const
{
    if (!GetType() && m_Name.empty())
        return os << "<null>";
    return GetType()->Print(os) << " %" << m_Name;
}

const std::string& Brewer::NamedValue::GetName() const
{
    return m_Name;
}

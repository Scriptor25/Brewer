#include <Brewer/Type.hpp>
#include <Brewer/Value/NamedValue.hpp>

Brewer::NamedValue::NamedValue(Type* type, std::string name)
    : Value(type), m_Name(std::move(name))
{
}

std::ostream& Brewer::NamedValue::PrintOperandIR(std::ostream& os, const bool omit_type) const
{
    if (!omit_type) GetType()->Print(os) << ' ';
    return os << '%' << GetName();
}

std::string Brewer::NamedValue::GetName() const
{
    return m_Name;
}

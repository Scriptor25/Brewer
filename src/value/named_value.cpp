#include <Brewer/Type.hpp>
#include <Brewer/Value/NamedValue.hpp>

Brewer::NamedValue::NamedValue(Type* type, std::string name, std::vector<std::string>&& meta)
    : Value(type, std::move(meta)), m_Name(std::move(name))
{
}

std::string Brewer::NamedValue::GetName() const
{
    return m_Name;
}

bool Brewer::NamedValue::RequiresDestination() const
{
    return true;
}

std::ostream& Brewer::NamedValue::PrintOperandIR(std::ostream& os, const bool omit_type) const
{
    if (!omit_type) GetType()->Print(os) << ' ';
    return os << '%' << GetName();
}

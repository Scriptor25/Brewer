#include <map>
#include <Brewer/Context.hpp>
#include <Brewer/Type.hpp>
#include <Brewer/Value/GlobalValue.hpp>

Brewer::GlobalValue::GlobalValue(Type* type, std::string name, const Linkage linkage)
    : NamedValue(type->GetContext().GetPointerType(type), std::move(name)), m_Linkage(linkage)
{
}

std::ostream& Brewer::GlobalValue::PrintOperandIR(std::ostream& os, const bool omit_type) const
{
    if (!omit_type) GetType()->Print(os) << ' ';
    return os << '@' << GetName();
}

Brewer::GlobalValue::Linkage Brewer::GlobalValue::GetLinkage() const
{
    return m_Linkage;
}

Brewer::GlobalValue::Linkage Brewer::ToLinkage(const std::string& str)
{
    static std::map<std::string, GlobalValue::Linkage> map
    {
        {"local", GlobalValue::Linkage_Local},
        {"weak", GlobalValue::Linkage_Weak},
        {"global", GlobalValue::Linkage_Global},
    };
    return map[str];
}

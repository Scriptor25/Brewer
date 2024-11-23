#include <utility>
#include <Brewer/Context.hpp>
#include <Brewer/Type.hpp>
#include <Brewer/Value/GlobalValue.hpp>

Brewer::GlobalValue::GlobalValue(Type* element_type, std::string name, const LinkageType linkage)
    : NamedValue(element_type->GetContext().GetPointerType(element_type), std::move(name)), m_Linkage(linkage)
{
}

std::ostream& Brewer::GlobalValue::PrintIR(std::ostream& os) const
{
    return PrintIROperand(os);
}

std::ostream& Brewer::GlobalValue::PrintIROperand(std::ostream& os) const
{
    return GetType()->Print(os) << " @" << GetName();
}

Brewer::GlobalValue::LinkageType Brewer::GlobalValue::GetLinkage() const
{
    return m_Linkage;
}

Brewer::GlobalValue::LinkageType Brewer::ToLinkage(const std::string& name)
{
    static std::map<std::string, GlobalValue::LinkageType> linkages
    {
        {"none", GlobalValue::NoLinkage},
        {"extern", GlobalValue::ExternLinkage},
        {"intern", GlobalValue::InternLinkage},
        {"weak", GlobalValue::WeakLinkage},
        {"common", GlobalValue::CommonLinkage},
        {"tentative", GlobalValue::TentativeLinkage},
    };

    if (!linkages.contains(name))
        Error("ToLinkage missing for linkage type '{}'", name);
    return linkages[name];
}

std::string Brewer::ToString(const GlobalValue::LinkageType linkage)
{
    static std::map<GlobalValue::LinkageType, std::string> linkages
    {
        {GlobalValue::NoLinkage, "none"},
        {GlobalValue::ExternLinkage, "extern"},
        {GlobalValue::InternLinkage, "intern"},
        {GlobalValue::WeakLinkage, "weak"},
        {GlobalValue::CommonLinkage, "common"},
        {GlobalValue::TentativeLinkage, "tentative"},
    };

    if (!linkages.contains(linkage))
        Error("ToString missing for linkage type {}", static_cast<unsigned>(linkage));
    return linkages[linkage];
}

std::ostream& Brewer::operator<<(std::ostream& os, const GlobalValue::LinkageType linkage)
{
    return os << ToString(linkage);
}

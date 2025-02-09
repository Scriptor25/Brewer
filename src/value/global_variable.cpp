#include <Brewer/Value/Constant.hpp>
#include <Brewer/Value/GlobalValue.hpp>

Brewer::GlobalVariable::GlobalVariable(
    Type *type,
    std::string name,
    const Linkage linkage,
    Constant *init,
    std::vector<std::string> &&meta)
    : GlobalValue(type, std::move(name), linkage, std::move(meta)),
      m_Init(init)
{
}

Brewer::Constant *Brewer::GlobalVariable::GetInit() const
{
    return m_Init;
}

std::ostream &Brewer::GlobalVariable::PrintIR(std::ostream &os) const
{
    os << "var ";
    if (GetLinkage() != Linkage_Local)
        os << ToString(GetLinkage()) << ' ';
    os << '@' << GetName() << " = ";
    return m_Init->PrintOperandIR(os, false);
}

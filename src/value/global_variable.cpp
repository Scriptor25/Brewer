#include <utility>
#include <Brewer/Type.hpp>
#include <Brewer/Value/Constant.hpp>
#include <Brewer/Value/GlobalVariable.hpp>

Brewer::GlobalVariable::GlobalVariable(Type* type, std::string name, const LinkageType linkage, Constant* initializer)
    : GlobalValue(type, std::move(name), linkage), m_Initializer(initializer)
{
}

std::ostream& Brewer::GlobalVariable::Print(std::ostream& os) const
{
    return m_Initializer->PrintOperand(os << "global " << GetLinkage() << " @" << GetName() << " = ");
}

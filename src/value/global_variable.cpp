#include <Brewer/Value/GlobalValue.hpp>

Brewer::GlobalVariable::GlobalVariable(Type* type, std::string name, const Linkage linkage, Constant* init)
    : GlobalValue(type, std::move(name), linkage), m_Init(init)
{
}

Brewer::Constant* Brewer::GlobalVariable::GetInit() const
{
    return m_Init;
}

#include <Brewer/Value/Constant.hpp>
#include <Brewer/Value/GlobalValue.hpp>

Brewer::GlobalVariable::GlobalVariable(Type* type, std::string name, const Linkage linkage, Constant* init)
    : GlobalValue(type, std::move(name), linkage), m_Init(init)
{
    m_Init->AddUse(this);
}

Brewer::Constant* Brewer::GlobalVariable::GetInit() const
{
    return m_Init;
}

void Brewer::GlobalVariable::Replace(Value* old_value, Value* new_value)
{
    if (m_Init == old_value) old_value = new_value;
}

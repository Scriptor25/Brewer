#include <Brewer/Type.hpp>
#include <Brewer/Value/Constant.hpp>

Brewer::ConstantInt::ConstantInt(IntType* type, const uint64_t val)
    : Constant(type), m_Val(val)
{
}

uint64_t Brewer::ConstantInt::GetVal() const
{
    return m_Val;
}

std::ostream& Brewer::ConstantInt::PrintOperandIR(std::ostream& os, const bool omit_type) const
{
    if (!omit_type) GetType()->Print(os) << ' ';
    return os << m_Val;
}

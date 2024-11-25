#include <Brewer/Type.hpp>
#include <Brewer/Value/Constant.hpp>

Brewer::ConstantFloat::ConstantFloat(FloatType* type, const double val)
    : Constant(type), m_Val(val)
{
}

double Brewer::ConstantFloat::GetVal() const
{
    return m_Val;
}

std::ostream& Brewer::ConstantFloat::PrintOperandIR(std::ostream& os, const bool omit_type) const
{
    if (!omit_type) GetType()->Print(os) << ' ';
    return os << m_Val;
}

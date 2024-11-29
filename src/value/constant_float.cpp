#include <Brewer/Type.hpp>
#include <Brewer/Value/Constant.hpp>

Brewer::ConstantFloat::ConstantFloat(FloatType* type, const double val, std::vector<std::string>&& meta)
    : Constant(type, std::move(meta)), m_Val(val)
{
}

double Brewer::ConstantFloat::GetVal() const
{
    return m_Val;
}

bool Brewer::ConstantFloat::NotNull() const
{
    return m_Val != 0.0;
}

std::ostream& Brewer::ConstantFloat::PrintOperandIR(std::ostream& os, const bool omit_type) const
{
    if (!omit_type) GetType()->Print(os) << ' ';
    return os << m_Val;
}

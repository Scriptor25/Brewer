#include <utility>
#include <Brewer/Type.hpp>
#include <Brewer/Value/Constant.hpp>

Brewer::Constant::Constant(Type* type)
    : Value(type)
{
}

std::ostream& Brewer::Constant::Print(std::ostream& os) const
{
    return PrintOperand(os);
}

Brewer::ConstantInt::ConstantInt(IntType* type, const uint64_t val)
    : Constant(type), m_Val(val)
{
}

std::ostream& Brewer::ConstantInt::PrintOperand(std::ostream& os) const
{
    return GetType()->Print(os) << ' ' << m_Val;
}

Brewer::ConstantFloat::ConstantFloat(FloatType* type, const double val)
    : Constant(type), m_Val(val)
{
}

std::ostream& Brewer::ConstantFloat::PrintOperand(std::ostream& os) const
{
    return GetType()->Print(os) << ' ' << m_Val;
}

Brewer::ConstantArray::ConstantArray(ArrayType* type, std::vector<Constant*> elements)
    : Constant(type), m_Elements(std::move(elements))
{
}

std::ostream& Brewer::ConstantArray::PrintOperand(std::ostream& os) const
{
    GetType()->Print(os) << " [";
    for (unsigned i = 0; i < m_Elements.size(); ++i)
    {
        if (i > 0) os << ", ";
        m_Elements[i]->PrintOperand(os);
    }
    return os << ']';
}

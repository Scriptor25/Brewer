#include <utility>
#include <Brewer/Type.hpp>
#include <Brewer/Value/Constant.hpp>

Brewer::Constant::Constant(Type* type)
    : Value(type)
{
}


std::ostream& Brewer::Constant::PrintIR(std::ostream& os) const
{
    return PrintIROperand(os);
}

Brewer::ConstantInt::ConstantInt(IntType* type, const uint64_t val)
    : Constant(type), m_Val(val)
{
}

std::ostream& Brewer::ConstantInt::PrintIROperand(std::ostream& os) const
{
    return GetType()->Print(os) << ' ' << m_Val;
}

uint64_t Brewer::ConstantInt::GetVal() const
{
    return m_Val;
}

Brewer::ConstantFloat::ConstantFloat(FloatType* type, const double val)
    : Constant(type), m_Val(val)
{
}

std::ostream& Brewer::ConstantFloat::PrintIROperand(std::ostream& os) const
{
    return GetType()->Print(os) << ' ' << m_Val;
}

double Brewer::ConstantFloat::GetVal() const
{
    return m_Val;
}

Brewer::ConstantArray::ConstantArray(ArrayType* type, std::vector<Constant*> elements)
    : Constant(type), m_Elements(std::move(elements))
{
}

std::ostream& Brewer::ConstantArray::PrintIROperand(std::ostream& os) const
{
    GetType()->Print(os) << ' ';

    if (GetType<ArrayType>()->GetElementType<IntType>()->Bits() == 8)
    {
        os << '"';
        for (const auto& element : m_Elements)
            if (const auto i = dynamic_cast<ConstantInt*>(element)->GetVal(); i >= 0x20)
                os << static_cast<char>(i);
            else os << '\\' << std::oct << i << std::dec;
        return os << '"';
    }

    os << '[';
    for (unsigned i = 0; i < m_Elements.size(); ++i)
    {
        if (i > 0) os << ", ";
        m_Elements[i]->PrintIROperand(os);
    }
    return os << ']';
}

unsigned Brewer::ConstantArray::GetNumElements() const
{
    return m_Elements.size();
}

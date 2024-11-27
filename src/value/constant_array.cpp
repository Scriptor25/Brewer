#include <Brewer/Type.hpp>
#include <Brewer/Value/Constant.hpp>

Brewer::ConstantArray::ConstantArray(ArrayType* type, std::vector<Constant*> vals)
    : Constant(type), m_Vals(std::move(vals))
{
    for (const auto& val : m_Vals)
        val->AddUse(this);
}

Brewer::Constant* Brewer::ConstantArray::GetVal(const unsigned i) const
{
    return m_Vals[i];
}

unsigned Brewer::ConstantArray::GetNumVals() const
{
    return m_Vals.size();
}

void Brewer::ConstantArray::Replace(Value* old_value, Value* new_value)
{
    Brewer::Replace(m_Vals, old_value, new_value);
}

std::ostream& Brewer::ConstantArray::PrintOperandIR(std::ostream& os, const bool omit_type) const
{
    if (!omit_type) GetType()->Print(os) << ' ';
    os << '[';
    for (unsigned i = 0; i < m_Vals.size(); ++i)
    {
        if (i > 0) os << ", ";
        m_Vals[i]->PrintOperandIR(os, true);
    }
    return os << ']';
}

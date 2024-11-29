#include <Brewer/Type.hpp>
#include <Brewer/Value/Constant.hpp>

#include "Brewer/Context.hpp"

Brewer::ConstantArray::ConstantArray(ArrayType* type, std::vector<Constant*> vals, std::vector<std::string>&& meta)
    : Constant(type, std::move(meta)), m_Vals(std::move(vals))
{
}

Brewer::Constant* Brewer::ConstantArray::GetVal(const unsigned i) const
{
    return m_Vals[i];
}

unsigned Brewer::ConstantArray::GetNumVals() const
{
    return m_Vals.size();
}

std::ostream& Brewer::ConstantArray::PrintOperandIR(std::ostream& os, const bool omit_type) const
{
    if (!omit_type) GetType()->Print(os) << ' ';

    if (GetType()->GetElementType() == GetType()->GetContext().GetIntNType(8))
    {
        os << '"';
        for (const auto val : m_Vals)
        {
            const auto i = dynamic_cast<ConstantInt*>(val)->GetVal();
            if (const auto c = static_cast<char>(i); c >= 0x20)
            {
                os << c;
            }
            else
            {
                const auto x0 = c / 8;
                const auto x1 = c % 8;
                os << '\\' << x0 << x1;
            }
        }
        return os << '"';
    }

    os << '[';
    for (unsigned i = 0; i < m_Vals.size(); ++i)
    {
        if (i > 0) os << ", ";
        m_Vals[i]->PrintOperandIR(os, true);
    }
    return os << ']';
}

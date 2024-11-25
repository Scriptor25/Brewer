#include <Brewer/Value/Value.hpp>

Brewer::Assignment::Assignment(Value* dst, Value* src)
    : Value(dst->GetType()), m_Dst(dst), m_Src(src)
{
}

std::ostream& Brewer::Assignment::PrintIR(std::ostream& os) const
{
    return m_Src->PrintIR(m_Dst->PrintOperandIR(os, true) << " = ");
}

std::ostream& Brewer::Assignment::PrintOperandIR(std::ostream& os, const bool omit_type) const
{
    return m_Dst->PrintOperandIR(os, omit_type);
}

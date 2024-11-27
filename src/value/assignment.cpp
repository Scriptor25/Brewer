#include <Brewer/Value/Value.hpp>

Brewer::Assignment::Assignment(Value* dst, Value* src)
    : Value(dst->GetType()), m_Dst(dst), m_Src(src)
{
    m_Dst->AddUse(this);
    m_Src->AddUse(this);
}

Brewer::Value* Brewer::Assignment::GetDst() const
{
    return m_Dst;
}

Brewer::Value* Brewer::Assignment::GetSrc() const
{
    return m_Src;
}

void Brewer::Assignment::Replace(Value* old_value, Value* new_value)
{
    if (old_value == m_Dst) m_Dst = new_value;
    if (old_value == m_Src) m_Src = new_value;
}

std::ostream& Brewer::Assignment::PrintIR(std::ostream& os) const
{
    return m_Src->PrintIR(m_Dst->PrintOperandIR(os, true) << " = ");
}

std::ostream& Brewer::Assignment::PrintOperandIR(std::ostream& os, const bool omit_type) const
{
    return m_Dst->PrintOperandIR(os, omit_type);
}

#include <map>
#include <utility>
#include <Brewer/Type.hpp>
#include <Brewer/Value/Instruction.hpp>

Brewer::Instruction::Instruction(Type* type, std::string name, const Code code, std::vector<Value*> operands)
    : NamedValue(type, std::move(name)), m_Code(code), m_Operands(std::move(operands))
{
    for (const auto& operand : m_Operands)
        operand->AddUse(this);
}

std::ostream& Brewer::Instruction::PrintIR(std::ostream& os) const
{
    if (!GetName().empty())
        GetType()->Print(os << '%' << GetName() << " = ") << ' ';

    os << m_Code << ' ';
    for (unsigned i = 0; i < m_Operands.size(); ++i)
    {
        if (i > 0) os << ", ";
        m_Operands[i]->PrintIROperand(os);
    }
    return os;
}

void Brewer::Instruction::ReplaceUseOf(Value* old_value, Value* new_value)
{
    for (auto& operand : m_Operands)
        if (operand == old_value)
        {
            operand = new_value;
            break;
        }
}

Brewer::Instruction::Code Brewer::Instruction::GetCode() const
{
    return m_Code;
}

Brewer::Value* Brewer::Instruction::GetOperand(const unsigned i) const
{
    return m_Operands[i];
}

unsigned Brewer::Instruction::GetNumOperands() const
{
    return m_Operands.size();
}

std::vector<Brewer::Value*> Brewer::Instruction::GetSubOperands(const unsigned begin, unsigned end) const
{
    if (!end) end = m_Operands.size();
    std::vector<Value*> sub_range;
    for (unsigned i = begin; i < end; ++i)
        sub_range.push_back(m_Operands[i]);
    return sub_range;
}

Brewer::Instruction::Code Brewer::ToCode(const std::string& name)
{
    static std::map<std::string, Instruction::Code> codes
    {
        {"isub", Instruction::ISub},
        {"iadd", Instruction::IAdd},
        {"call", Instruction::Call},
        {"gep", Instruction::GEP},
        {"phi", Instruction::PHI},
        {"ret", Instruction::Ret},
        {"br", Instruction::Br},
        {"br.lt", Instruction::Br_LT},
    };

    if (!codes.contains(name))
        Error("ToCode missing for instruction code '{}'", name);
    return codes[name];
}

std::string Brewer::ToString(const Instruction::Code code)
{
    static std::map<Instruction::Code, std::string> codes
    {
        {Instruction::ISub, "isub"},
        {Instruction::IAdd, "iadd"},
        {Instruction::Call, "call"},
        {Instruction::GEP, "gep"},
        {Instruction::PHI, "phi"},
        {Instruction::Ret, "ret"},
        {Instruction::Br, "br"},
        {Instruction::Br_LT, "br.lt"},
    };

    if (!codes.contains(code))
        Error("ToString missing for instruction code {}", static_cast<unsigned>(code));
    return codes[code];
}

std::ostream& Brewer::operator<<(std::ostream& os, const Instruction::Code code)
{
    return os << ToString(code);
}

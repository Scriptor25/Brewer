#include <map>
#include <Brewer/Type.hpp>
#include <Brewer/Value/Value.hpp>

Brewer::Instruction::Instruction(Type* type, const Code code, std::vector<Value*> operands)
    : Value(type), m_Code(code), m_Operands(std::move(operands))
{
}

std::ostream& Brewer::Instruction::PrintOperandIR(std::ostream& os, bool omit_type) const
{
    if (!omit_type) GetType()->Print(os) << ' ';
    os << ToString(m_Code) << ' ';
    for (unsigned i = 0; i < m_Operands.size(); ++i)
    {
        if (i > 0) os << ", ";
        m_Operands[i]->PrintOperandIR(os, false);
    }
    return os;
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

std::vector<Brewer::Value*> Brewer::Instruction::GetOperandRange(unsigned beg, unsigned end) const
{
    std::vector<Value*> result;
    for (unsigned i = beg; i < end && i < m_Operands.size(); ++i)
        result.push_back(m_Operands[i]);
    return result;
}

Brewer::Instruction::Code Brewer::ToCode(const std::string& str)
{
    static std::map<std::string, Instruction::Code> map
    {
        {"add", Instruction::Add},
        {"sub", Instruction::Sub},
        {"call", Instruction::Call},
        {"gep", Instruction::Gep},
        {"ret", Instruction::Ret},
        {"br", Instruction::Br},
        {"br.lt", Instruction::Br_Lt},
    };
    if (!map.contains(str))
        return Instruction::None;
    return map[str];
}

std::string Brewer::ToString(const Instruction::Code code)
{
    static std::map<Instruction::Code, std::string> map
    {
        {Instruction::Add, "add"},
        {Instruction::Sub, "sub"},
        {Instruction::Call, "call"},
        {Instruction::Gep, "gep"},
        {Instruction::Ret, "ret"},
        {Instruction::Br, "br"},
        {Instruction::Br_Lt, "br.lt"},
    };
    return map[code];
}

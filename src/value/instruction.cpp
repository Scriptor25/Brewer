#include <map>
#include <Brewer/Type.hpp>
#include <Brewer/Value/Value.hpp>

#include "Brewer/Value/Constant.hpp"

Brewer::Instruction::Instruction(
    Type *type,
    const Code code,
    std::vector<Value *> operands,
    std::vector<std::string> &&meta)
    : Value(type, std::move(meta)),
      m_Code(code),
      m_Operands(std::move(operands))
{
    for (const auto &operand: m_Operands)
        operand->AddUse(this);
}

std::ostream &Brewer::Instruction::PrintOperandIR(std::ostream &os, const bool omit_type) const
{
    if (!omit_type)
        GetType()->Print(os) << ' ';
    os << ToString(m_Code) << ' ';
    for (unsigned i = 0; i < m_Operands.size(); ++i)
    {
        if (i > 0)
            os << ", ";
        m_Operands[i]->PrintOperandIR(os, false);
    }
    return os;
}

Brewer::Instruction::Code Brewer::Instruction::GetCode() const
{
    return m_Code;
}

Brewer::Value *Brewer::Instruction::GetOperand(const unsigned i) const
{
    return m_Operands[i];
}

unsigned Brewer::Instruction::GetNumOperands() const
{
    return m_Operands.size();
}

std::vector<Brewer::Value *> Brewer::Instruction::GetOperandRange(const unsigned beg, const unsigned end) const
{
    std::vector<Value *> result;
    for (auto i = beg; i < end && i < m_Operands.size(); ++i)
        result.push_back(m_Operands[i]);
    return result;
}

bool Brewer::Instruction::IsTerminator() const
{
    switch (m_Code)
    {
        case Ret:
        case Br:
        case Br_Lt:
            return true;
        default:
            return false;
    }
}

bool Brewer::Instruction::RequiresDestination() const
{
    switch (m_Code)
    {
        case Add:
        case Sub:
        case Gep:
        case Load:
        case Alloca:
            return true;
        default:
            return false;
    }
}

uint64_t Brewer::Instruction::GetNumAllocaBytes() const
{
    uint64_t bytes = 0;
    switch (m_Code)
    {
        case Call:
            bytes = (m_Operands.size() - 1) * 8;
            if (bytes < 32)
                bytes = 32; // shadow
            break;

        case Alloca:
            bytes = GetType()->GetElementType()->GetNumBytes();
            bytes *= dynamic_cast<ConstantInt *>(m_Operands[0])->GetVal();
            if (const auto rem = bytes % 0x10)
                bytes += 0x10 - rem;
            break;

        default:
            break;
    }
    return bytes;
}

void Brewer::Instruction::Replace(Value *old_value, Value *new_value)
{
    Brewer::Replace(m_Operands, old_value, new_value);
}

Brewer::Instruction::Code Brewer::ToCode(const std::string &str)
{
    static std::map<std::string, Instruction::Code> map
    {
        {"add", Instruction::Add},
        {"sub", Instruction::Sub},
        {"call", Instruction::Call},
        {"gep", Instruction::Gep},
        {"load", Instruction::Load},
        {"store", Instruction::Store},
        {"alloca", Instruction::Alloca},
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
        {Instruction::Load, "load"},
        {Instruction::Store, "store"},
        {Instruction::Alloca, "alloca"},
        {Instruction::Ret, "ret"},
        {Instruction::Br, "br"},
        {Instruction::Br_Lt, "br.lt"},
    };
    return map[code];
}

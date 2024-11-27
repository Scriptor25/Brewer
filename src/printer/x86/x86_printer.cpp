#include <Brewer/Type.hpp>
#include <Brewer/Printer/X86Printer.hpp>
#include <Brewer/Value/Value.hpp>

const std::vector<Brewer::X86Printer::Register> Brewer::X86Printer::CALL_REGISTERS
{
    RCX,
    RDX,
    R8,
    R9,
};

static std::string suffix(const std::string& str, const unsigned bytes)
{
    static std::map<unsigned, char> s
    {
        {1, 'b'},
        {2, 'w'},
        {4, 'l'},
        {8, 'q'},
    };
    return str + s[bytes];
}

static std::string to_string(const Brewer::X86Printer::Register reg, const unsigned bytes)
{
    static std::map<Brewer::X86Printer::Register, std::string> gp_regs
    {
        {Brewer::X86Printer::RAX, "a"},
        {Brewer::X86Printer::RBX, "b"},
        {Brewer::X86Printer::RCX, "c"},
        {Brewer::X86Printer::RDX, "d"},
    };
    static std::map<Brewer::X86Printer::Register, std::string> sp_regs
    {
        {Brewer::X86Printer::RSI, "si"},
        {Brewer::X86Printer::RDI, "di"},
        {Brewer::X86Printer::RSP, "sp"},
        {Brewer::X86Printer::RBP, "bp"},
    };
    static std::map<Brewer::X86Printer::Register, std::string> ex_regs
    {
        {Brewer::X86Printer::R8, "r8"},
        {Brewer::X86Printer::R9, "r9"},
        {Brewer::X86Printer::R10, "r10"},
        {Brewer::X86Printer::R11, "r11"},
        {Brewer::X86Printer::R12, "r12"},
        {Brewer::X86Printer::R13, "r13"},
        {Brewer::X86Printer::R14, "r14"},
        {Brewer::X86Printer::R15, "r15"},
    };

    if (gp_regs.contains(reg))
    {
        std::string res = gp_regs[reg];
        switch (bytes)
        {
        case 0:
            res = res + 'h';
            break;
        case 1:
            res = res + 'l';
            break;
        case 2:
            res = res + 'x';
            break;
        case 4:
            res = 'e' + res + 'x';
            break;
        case 8:
            res = 'r' + res + 'x';
            break;
        default: break;
        }
        return res;
    }
    if (sp_regs.contains(reg))
    {
        std::string res = sp_regs[reg];
        switch (bytes)
        {
        case 0:
        case 1:
            res = res + 'l';
            break;
        case 2: break;
        case 4:
            res = 'e' + res;
            break;
        case 8:
            res = 'r' + res;
            break;
        default: break;
        }
        return res;
    }

    std::string res = ex_regs[reg];
    switch (bytes)
    {
    case 0:
    case 1:
        res += 'b';
        break;
    case 2:
        res += 'w';
        break;
    case 4:
        res += 'd';
        break;
    default: break;
    }
    return res;
}

bool Brewer::X86Printer::Storage::Equal(const Storage* lhs, const Storage* rhs)
{
    if (lhs == rhs) return true;
    if (lhs == nullptr || rhs == nullptr) return false;
    return lhs->Equals(rhs);
}

Brewer::X86Printer::RegisterStorage::RegisterStorage(const Register reg)
    : Reg(reg)
{
}

void Brewer::X86Printer::RegisterStorage::Print(X86Printer& printer, const unsigned bytes) const
{
    printer.S() << '%' << to_string(Reg, bytes);
}

bool Brewer::X86Printer::RegisterStorage::Equals(const Storage* other) const
{
    if (const auto reg = dynamic_cast<const RegisterStorage*>(other))
        return reg->Reg == Reg;
    return false;
}

Brewer::X86Printer::MemoryStorage::MemoryStorage(
    const int segment,
    const int displacement,
    const Register base,
    const Register index,
    const int scale)
    : Segment(segment), Displacement(displacement), Base(base), Index(index), Scale(scale)
{
}

void Brewer::X86Printer::MemoryStorage::Print(X86Printer& printer, unsigned) const
{
    if (Segment) printer.S() << Segment << ':';
    if (Displacement) printer.S() << Displacement;
    printer.S() << '(';
    if (Base) printer.S() << '%' << to_string(Base, 8);
    if (!Index)
    {
        printer.S() << ')';
        return;
    }
    printer.S() << ",%" << to_string(Index, 8);
    if (!Scale)
    {
        printer.S() << ')';
        return;
    }
    printer.S() << ',' << Scale << ')';
}

bool Brewer::X86Printer::MemoryStorage::Equals(const Storage* other) const
{
    if (const auto mem = dynamic_cast<const MemoryStorage*>(other))
        return mem->Segment == Segment
            && mem->Displacement == Displacement
            && mem->Base == Base
            && mem->Index == Index
            && mem->Scale == Scale;
    return false;
}

Brewer::X86Printer::ImmediateStorage::ImmediateStorage(const int value)
    : Value(value)
{
}

void Brewer::X86Printer::ImmediateStorage::Print(X86Printer& printer, unsigned) const
{
    printer.S() << '$' << Value;
}

bool Brewer::X86Printer::ImmediateStorage::Equals(const Storage* other) const
{
    if (const auto imm = dynamic_cast<const ImmediateStorage*>(other))
        return imm->Value == Value;
    return false;
}

Brewer::X86Printer::X86Printer(std::ostream& stream)
    : Printer(stream)
{
}

void Brewer::X86Printer::op(const std::string& name, const std::vector<const Storage*>& operands, const unsigned bytes)
{
    S() << '\t' << suffix(name, bytes) << ' ';
    for (unsigned i = 0; i < operands.size(); ++i)
    {
        if (i > 0) S() << ", ";
        operands[i]->Print(*this, bytes);
    }
    S() << std::endl;
}

void Brewer::X86Printer::mov(const Storage* src, const Storage* dst, const unsigned bytes)
{
    if (Storage::Equal(src, dst)) return;
    op("mov", {src, dst}, bytes);
}

void Brewer::X86Printer::push(const Storage* src, const unsigned bytes)
{
    op("push", {src}, bytes);
}

void Brewer::X86Printer::pop(const Storage* dst, const unsigned bytes)
{
    op("pop", {dst}, bytes);
}

void Brewer::X86Printer::add(const Storage* src, const Storage* dst, const unsigned bytes)
{
    op("add", {src, dst}, bytes);
}

void Brewer::X86Printer::sub(const Storage* src, const Storage* dst, unsigned bytes)
{
    op("sub", {src, dst}, bytes);
}

void Brewer::X86Printer::imul(const Storage* src, const Storage* dst, unsigned bytes)
{
    op("imul", {src, dst}, bytes);
}

void Brewer::X86Printer::cmp(const Storage* l, const Storage* r, const unsigned bytes)
{
    op("cmp", {l, r}, bytes);
}

void Brewer::X86Printer::lea(const Storage* src, const Storage* dst, unsigned bytes)
{
    op("lea", {src, dst}, bytes);
}

void Brewer::X86Printer::op(const std::string& name, Value* value, const Storage* dst)
{
    const auto bytes = value->GetType()->CountBytes();
    S() << '\t' << suffix(name, bytes) << ' ';
    PrintOperand(value);
    if (dst)
    {
        S() << ", ";
        dst->Print(*this, bytes);
    }
    S() << std::endl;
}

void Brewer::X86Printer::mov(Value* value, const Storage* dst)
{
    op("mov", value, dst);
}

void Brewer::X86Printer::push(Value* value)
{
    op("push", value);
}

void Brewer::X86Printer::pop(Value* value)
{
    op("pop", value);
}

void Brewer::X86Printer::add(Value* value, const Storage* dst)
{
    op("add", value, dst);
}

void Brewer::X86Printer::sub(Value* value, const Storage* dst)
{
    op("sub", value, dst);
}

void Brewer::X86Printer::imul(Value* value, const Storage* dst)
{
    op("imul", value, dst);
}

void Brewer::X86Printer::cmp(Value* value, const Storage* dst)
{
    op("cmp", value, dst);
}

void Brewer::X86Printer::lea(Value* value, const Storage* dst)
{
    op("lea", value, dst);
}

void Brewer::X86Printer::ret() const
{
    S() << '\t' << "ret" << std::endl;
}

void Brewer::X86Printer::call(Value* value, const Storage* dst)
{
    S() << '\t' << "call ";
    PrintCallee(value);
    S() << std::endl;

    if (!dst) return;
    const RegisterStorage rax(RAX);
    const auto bytes = value->GetType()->GetElementType()->GetResultType()->CountBytes();
    mov(&rax, dst, bytes);
}

void Brewer::X86Printer::jmp(Value* value)
{
    S() << '\t' << "jmp ";
    PrintCallee(value);
    S() << std::endl;
}

void Brewer::X86Printer::jl(Value* value)
{
    S() << '\t' << "jl ";
    PrintCallee(value);
    S() << std::endl;
}

void Brewer::X86Printer::BeginFrame()
{
    m_Offsets.clear();
    m_Top = 0;
}

int Brewer::X86Printer::GetOffset(Value* value)
{
    if (m_Offsets.contains(value))
        return m_Offsets[value];

    m_Top -= static_cast<int>(value->GetType()->CountBytes());
    return m_Offsets[value] = m_Top;
}

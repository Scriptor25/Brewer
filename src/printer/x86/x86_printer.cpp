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

bool Brewer::X86Printer::Storage::Equal(const Storage& lhs, const Storage& rhs)
{
    if (!lhs || !rhs) return false;
    if (lhs.Immediate != rhs.Immediate) return false;
    if (lhs.Direct != rhs.Direct) return false;
    if (lhs.Immediate)
        return lhs.Displacement == rhs.Displacement;
    if (lhs.Direct)
        return lhs.Base == rhs.Base;
    return lhs.Segment == rhs.Segment
        && lhs.Displacement == rhs.Displacement
        && lhs.Base == rhs.Base
        && lhs.Index == rhs.Index
        && lhs.Scale == rhs.Scale;
}

Brewer::X86Printer::Storage::Storage() = default;

Brewer::X86Printer::Storage::Storage(const int64_t value)
    : Valid(true), Immediate(true), Displacement(value)
{
}

Brewer::X86Printer::Storage::Storage(const uint64_t value)
    : Valid(true), Immediate(true), Displacement(value)
{
}

Brewer::X86Printer::Storage::Storage(const Register reg)
    : Valid(true), Direct(true), Base(reg)
{
}

Brewer::X86Printer::Storage::Storage(
    const int64_t segment,
    const int64_t displacement,
    const Register base,
    const Register index,
    const int64_t scale)
    : Valid(true),
      Segment(segment),
      Displacement(displacement),
      Base(base),
      Index(index),
      Scale(scale)
{
}

void Brewer::X86Printer::Storage::Print(const X86Printer& printer, const unsigned bytes) const
{
    if (!Valid)
    {
        printer.S() << "<invalid>";
        return;
    }
    if (Immediate)
    {
        printer.S() << '$' << Displacement;
        return;
    }
    if (Direct)
    {
        printer.S() << '%' << to_string(Base, bytes);
        return;
    }

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

Brewer::X86Printer::Storage::operator bool() const
{
    return Valid;
}

Brewer::X86Printer::X86Printer(std::ostream& stream)
    : Printer(stream)
{
}

bool Brewer::X86Printer::can_omit_mov(const Storage& src, const Storage& dst)
{
    if (Storage::Equal(src, dst))
        return true;
    if (Storage::Equal(src, m_LastSrc) && Storage::Equal(dst, m_LastDst))
        return true;
    if (Storage::Equal(dst, m_LastSrc) && Storage::Equal(src, m_LastDst))
        return true;
    return false;
}

void Brewer::X86Printer::set_last(const Storage& src, const Storage& dst)
{
    m_LastSrc = src;
    m_LastDst = dst;
}

void Brewer::X86Printer::clear_last()
{
    set_last({}, {});
}

void Brewer::X86Printer::op(const std::string& name, const std::vector<Storage>& operands, const unsigned bytes)
{
    S() << '\t' << suffix(name, bytes) << ' ';
    for (unsigned i = 0; i < operands.size(); ++i)
    {
        if (i > 0) S() << ", ";
        operands[i].Print(*this, bytes);
    }
    S() << std::endl;
}

void Brewer::X86Printer::mov(const Storage& src, const Storage& dst, const unsigned bytes)
{
    if (can_omit_mov(src, dst))
        return;
    set_last(src, dst);
    op("mov", {src, dst}, bytes);
}

void Brewer::X86Printer::push(const Storage& src, const unsigned bytes)
{
    op("push", {src}, bytes);
}

void Brewer::X86Printer::pop(const Storage& dst, const unsigned bytes)
{
    if (Storage::Equal(dst, m_LastDst))
        clear_last();
    op("pop", {dst}, bytes);
}

void Brewer::X86Printer::add(const Storage& src, const Storage& dst, const unsigned bytes)
{
    clear_last();
    op("add", {src, dst}, bytes);
}

void Brewer::X86Printer::sub(const Storage& src, const Storage& dst, const unsigned bytes)
{
    clear_last();
    op("sub", {src, dst}, bytes);
}

void Brewer::X86Printer::imul(const Storage& src, const Storage& dst, const unsigned bytes)
{
    clear_last();
    op("imul", {src, dst}, bytes);
}

void Brewer::X86Printer::cmp(const Storage& l, const Storage& r, const unsigned bytes)
{
    op("cmp", {l, r}, bytes);
}

void Brewer::X86Printer::lea(const Storage& src, const Storage& dst, const unsigned bytes)
{
    clear_last();
    op("lea", {src, dst}, bytes);
}

void Brewer::X86Printer::op(const std::string& name, Value* value, const Storage& dst)
{
    const auto bytes = value->GetType()->CountBytes();
    S() << '\t' << suffix(name, bytes) << ' ';
    PrintOperand(value);
    if (dst)
    {
        S() << ", ";
        dst.Print(*this, bytes);
    }
    S() << std::endl;
}

void Brewer::X86Printer::mov(Value* value, const Storage& dst)
{
    if (Storage::Equal(dst, m_LastDst))
        clear_last();
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

void Brewer::X86Printer::add(Value* value, const Storage& dst)
{
    if (Storage::Equal(dst, m_LastDst))
        clear_last();
    op("add", value, dst);
}

void Brewer::X86Printer::sub(Value* value, const Storage& dst)
{
    if (Storage::Equal(dst, m_LastDst))
        clear_last();
    op("sub", value, dst);
}

void Brewer::X86Printer::imul(Value* value, const Storage& dst)
{
    if (Storage::Equal(dst, m_LastDst))
        clear_last();
    op("imul", value, dst);
}

void Brewer::X86Printer::cmp(Value* value, const Storage& dst)
{
    clear_last();
    op("cmp", value, dst);
}

void Brewer::X86Printer::lea(Value* value, const Storage& dst)
{
    if (Storage::Equal(dst, m_LastDst))
        clear_last();
    op("lea", value, dst);
}

void Brewer::X86Printer::ret() const
{
    S() << '\t' << "ret" << std::endl;
}

void Brewer::X86Printer::call(Value* value, const Storage& dst)
{
    S() << '\t' << "call ";
    PrintCallee(value);
    S() << std::endl;

    if (!dst) return;
    const Storage rax(RAX);
    const auto bytes = value->GetType()->GetElementType()->GetResultType()->CountBytes();
    mov(rax, dst, bytes);
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
    clear_last();
    m_Offsets.clear();
    m_Offset = 0;
    m_Top = 0;
}

int64_t Brewer::X86Printer::GetOffset(Value* value)
{
    if (m_Offsets.contains(value))
        return m_Offsets[value];

    m_Offset -= value->GetType()->CountBytes();
    return m_Offsets[value] = m_Offset;
}

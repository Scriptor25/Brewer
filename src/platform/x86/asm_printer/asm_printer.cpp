#include <Brewer/Module.hpp>
#include <Brewer/Type.hpp>
#include <Brewer/Platform/X86/ASMPrinter.hpp>
#include <Brewer/Value/Value.hpp>

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

bool Brewer::Platform::X86::ASMPrinter::Storage::Equal(const Storage& lhs, const Storage& rhs)
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

Brewer::Platform::X86::ASMPrinter::Storage::Storage() = default;

Brewer::Platform::X86::ASMPrinter::Storage::Storage(const int64_t value)
    : Valid(true), Immediate(true), Displacement(value)
{
}

Brewer::Platform::X86::ASMPrinter::Storage::Storage(const uint64_t value)
    : Valid(true), Immediate(true), Displacement(static_cast<int64_t>(value))
{
}

Brewer::Platform::X86::ASMPrinter::Storage::Storage(const Register reg)
    : Valid(true), Direct(true), Base(reg)
{
}

Brewer::Platform::X86::ASMPrinter::Storage::Storage(
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

void Brewer::Platform::X86::ASMPrinter::Storage::Print(const ASMPrinter& printer, const unsigned bytes) const
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

Brewer::Platform::X86::ASMPrinter::Storage::operator bool() const
{
    return Valid;
}

std::string Brewer::Platform::X86::ASMPrinter::to_string(const Register reg, const unsigned bytes)
{
    static std::map<Register, std::string> gp_regs
    {
        {RA, "a"},
        {RB, "b"},
        {RC, "c"},
        {RD, "d"},
    };
    static std::map<Register, std::string> sp_regs
    {
        {RSI, "si"},
        {RDI, "di"},
        {RSP, "sp"},
        {RBP, "bp"},
    };
    static std::map<Register, std::string> ex_regs
    {
        {R8, "r8"},
        {R9, "r9"},
        {R10, "r10"},
        {R11, "r11"},
        {R12, "r12"},
        {R13, "r13"},
        {R14, "r14"},
        {R15, "r15"},
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

const std::vector<Brewer::Platform::X86::ASMPrinter::Register> Brewer::Platform::X86::ASMPrinter::CALL_REGISTERS
{
    RC, RD, R8, R9,
};

Brewer::Platform::X86::ASMPrinter::ASMPrinter(std::ostream& stream)
    : ASMPrinterBase(stream)
{
}

void Brewer::Platform::X86::ASMPrinter::Print(Module& module)
{
    S() << ".file \"" << module.GetFilename() << '"' << std::endl;
    S() << ".ident \"Brewer\"" << std::endl;
    module.ForEach([&](GlobalValue* value)
    {
        S() << std::endl;
        PrintGlobal(value);
    });
}

void Brewer::Platform::X86::ASMPrinter::Print(Value* value)
{
    return Print(value, {});
}

bool Brewer::Platform::X86::ASMPrinter::CanOmitMov(const Storage& src, const Storage& dst)
{
    if (Storage::Equal(src, dst))
        return true;
    if (Storage::Equal(src, m_LastSrc) && Storage::Equal(dst, m_LastDst))
        return true;
    if (Storage::Equal(dst, m_LastSrc) && Storage::Equal(src, m_LastDst))
        return true;
    return false;
}

void Brewer::Platform::X86::ASMPrinter::SetLast(const Storage& src, const Storage& dst)
{
    m_LastSrc = src;
    m_LastDst = dst;
}

void Brewer::Platform::X86::ASMPrinter::ClearLast()
{
    SetLast({}, {});
}

void Brewer::Platform::X86::ASMPrinter::asm_op(
    const std::string& name,
    const std::vector<Storage>& operands,
    const unsigned bytes)
{
    S() << '\t' << suffix(name, bytes) << ' ';
    for (unsigned i = 0; i < operands.size(); ++i)
    {
        if (i > 0) S() << ", ";
        operands[i].Print(*this, bytes);
    }
    S() << std::endl;
}

void Brewer::Platform::X86::ASMPrinter::asm_mov(const Storage& src, const Storage& dst, const unsigned bytes)
{
    if (CanOmitMov(src, dst))
        return;
    SetLast(src, dst);
    asm_op("mov", {src, dst}, bytes);
}

void Brewer::Platform::X86::ASMPrinter::asm_push(const Storage& src, const unsigned bytes)
{
    asm_op("push", {src}, bytes);
}

void Brewer::Platform::X86::ASMPrinter::asm_pop(const Storage& dst, const unsigned bytes)
{
    if (Storage::Equal(dst, m_LastDst))
        ClearLast();
    asm_op("pop", {dst}, bytes);
}

void Brewer::Platform::X86::ASMPrinter::asm_add(const Storage& src, const Storage& dst, const unsigned bytes)
{
    ClearLast();
    asm_op("add", {src, dst}, bytes);
}

void Brewer::Platform::X86::ASMPrinter::asm_sub(const Storage& src, const Storage& dst, const unsigned bytes)
{
    ClearLast();
    asm_op("sub", {src, dst}, bytes);
}

void Brewer::Platform::X86::ASMPrinter::asm_imul(const Storage& src, const Storage& dst, const unsigned bytes)
{
    ClearLast();
    asm_op("imul", {src, dst}, bytes);
}

void Brewer::Platform::X86::ASMPrinter::asm_cmp(const Storage& l, const Storage& r, const unsigned bytes)
{
    asm_op("cmp", {l, r}, bytes);
}

void Brewer::Platform::X86::ASMPrinter::asm_lea(const Storage& src, const Storage& dst, const unsigned bytes)
{
    ClearLast();
    asm_op("lea", {src, dst}, bytes);
}

void Brewer::Platform::X86::ASMPrinter::asm_op(const std::string& name, Value* value, const Storage& dst)
{
    const auto bytes = value->GetType()->GetNumBytes();
    S() << '\t' << suffix(name, bytes) << ' ';
    PrintOperand(value);
    if (dst)
    {
        S() << ", ";
        dst.Print(*this, bytes);
    }
    S() << std::endl;
}

void Brewer::Platform::X86::ASMPrinter::asm_mov(Value* value, const Storage& dst)
{
    if (Storage::Equal(dst, m_LastDst))
        ClearLast();
    asm_op("mov", value, dst);
}

void Brewer::Platform::X86::ASMPrinter::asm_push(Value* value)
{
    asm_op("push", value);
}

void Brewer::Platform::X86::ASMPrinter::asm_pop(Value* value)
{
    asm_op("pop", value);
}

void Brewer::Platform::X86::ASMPrinter::asm_add(Value* value, const Storage& dst)
{
    if (Storage::Equal(dst, m_LastDst))
        ClearLast();
    asm_op("add", value, dst);
}

void Brewer::Platform::X86::ASMPrinter::asm_sub(Value* value, const Storage& dst)
{
    if (Storage::Equal(dst, m_LastDst))
        ClearLast();
    asm_op("sub", value, dst);
}

void Brewer::Platform::X86::ASMPrinter::asm_imul(Value* value, const Storage& dst)
{
    if (Storage::Equal(dst, m_LastDst))
        ClearLast();
    asm_op("imul", value, dst);
}

void Brewer::Platform::X86::ASMPrinter::asm_cmp(Value* value, const Storage& dst)
{
    ClearLast();
    asm_op("cmp", value, dst);
}

void Brewer::Platform::X86::ASMPrinter::asm_lea(Value* value, const Storage& dst)
{
    if (Storage::Equal(dst, m_LastDst))
        ClearLast();
    asm_op("lea", value, dst);
}

void Brewer::Platform::X86::ASMPrinter::asm_ret() const
{
    S() << '\t' << "ret" << std::endl;
}

void Brewer::Platform::X86::ASMPrinter::asm_call(Value* value, const Storage& dst)
{
    S() << '\t' << "call ";
    PrintCallee(value);
    S() << std::endl;

    if (!dst) return;
    const Storage rax(RA);
    const auto bytes = value->GetType()->GetElementType()->GetResultType()->GetNumBytes();
    asm_mov(rax, dst, bytes);
}

void Brewer::Platform::X86::ASMPrinter::asm_jmp(Value* value)
{
    S() << '\t' << "jmp ";
    PrintCallee(value);
    S() << std::endl;
}

void Brewer::Platform::X86::ASMPrinter::asm_jl(Value* value)
{
    S() << '\t' << "jl ";
    PrintCallee(value);
    S() << std::endl;
}

void Brewer::Platform::X86::ASMPrinter::BeginFrame()
{
    ClearLast();
    m_Offsets.clear();
    m_Offset = 0;
    m_Top = 0;
}

int64_t Brewer::Platform::X86::ASMPrinter::GetOffset(Value* value)
{
    if (m_Offsets.contains(value))
        return m_Offsets[value];

    m_Offset -= value->GetType()->GetNumBytes();
    return m_Offsets[value] = m_Offset;
}

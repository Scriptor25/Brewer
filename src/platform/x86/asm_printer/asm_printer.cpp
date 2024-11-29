#include <Brewer/Module.hpp>
#include <Brewer/Type.hpp>
#include <Brewer/Platform/X86/ASMPrinter.hpp>
#include <Brewer/Value/Value.hpp>

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
        printer.S() << '%' << ToString(Base, bytes);
        return;
    }

    if (Segment) printer.S() << Segment << ':';
    if (Displacement) printer.S() << Displacement;
    printer.S() << '(';
    if (Base) printer.S() << '%' << ToString(Base, 8);
    if (!Index)
    {
        printer.S() << ')';
        return;
    }
    printer.S() << ",%" << ToString(Index, 8);
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

std::string Brewer::Platform::X86::ASMPrinter::ToString(const Register reg, const unsigned bytes)
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

Brewer::Platform::X86::ASMPrinter::Register Brewer::Platform::X86::ASMPrinter::GetCallRegister(
    const CallConv conv,
    const unsigned i)
{
    constexpr Register ms_x64[]{RC, RD, R8, R9};
    constexpr Register system_v[]{RDI, RSI, RD, RC, R8, R9};

    switch (conv)
    {
    case MS_X64: return ms_x64[i];
    case SYSTEM_V: return system_v[i];
    }

    Error("X86 - GetCallRegister({}, {})", static_cast<unsigned>(conv), i);
}

unsigned Brewer::Platform::X86::ASMPrinter::GetNumCallRegisters(const CallConv conv)
{
    switch (conv)
    {
    case MS_X64: return 4;
    case SYSTEM_V: return 6;
    }

    Error("X86 - GetNumCallRegisters({})", static_cast<unsigned>(conv));
}

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

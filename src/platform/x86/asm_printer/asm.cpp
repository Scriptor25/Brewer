#include <Brewer/Type.hpp>
#include <Brewer/Platform/X86/ASMPrinter.hpp>
#include <Brewer/Value/Value.hpp>

static std::string suffix(const std::string& str, const unsigned bytes)
{
    static std::map<unsigned, char> suffix
    {
        {1, 'b'},
        {2, 'w'},
        {4, 'l'},
        {8, 'q'},
    };
    return str + suffix[bytes];
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

void Brewer::Platform::X86::ASMPrinter::Asm(
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

void Brewer::Platform::X86::ASMPrinter::Mov(const Storage& src, const Storage& dst, const unsigned bytes)
{
    if (CanOmitMov(src, dst))
        return;
    SetLast(src, dst);
    Asm("mov", {src, dst}, bytes);
}

void Brewer::Platform::X86::ASMPrinter::Push(const Storage& src, const unsigned bytes)
{
    Asm("push", {src}, bytes);
}

void Brewer::Platform::X86::ASMPrinter::Pop(const Storage& dst, const unsigned bytes)
{
    if (Storage::Equal(dst, m_LastDst))
        ClearLast();
    Asm("pop", {dst}, bytes);
}

void Brewer::Platform::X86::ASMPrinter::Add(const Storage& src, const Storage& dst, const unsigned bytes)
{
    ClearLast();
    Asm("add", {src, dst}, bytes);
}

void Brewer::Platform::X86::ASMPrinter::Sub(const Storage& src, const Storage& dst, const unsigned bytes)
{
    ClearLast();
    Asm("sub", {src, dst}, bytes);
}

void Brewer::Platform::X86::ASMPrinter::IMul(const Storage& src, const Storage& dst, const unsigned bytes)
{
    ClearLast();
    Asm("imul", {src, dst}, bytes);
}

void Brewer::Platform::X86::ASMPrinter::Cmp(const Storage& l, const Storage& r, const unsigned bytes)
{
    Asm("cmp", {l, r}, bytes);
}

void Brewer::Platform::X86::ASMPrinter::Lea(const Storage& src, const Storage& dst, const unsigned bytes)
{
    ClearLast();
    Asm("lea", {src, dst}, bytes);
}

void Brewer::Platform::X86::ASMPrinter::Asm(const std::string& name, Value* value, const Storage& dst)
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

void Brewer::Platform::X86::ASMPrinter::Mov(Value* value, const Storage& dst)
{
    if (Storage::Equal(dst, m_LastDst))
        ClearLast();
    Asm("mov", value, dst);
}

void Brewer::Platform::X86::ASMPrinter::Push(Value* value)
{
    Asm("push", value);
}

void Brewer::Platform::X86::ASMPrinter::Pop(Value* value)
{
    Asm("pop", value);
}

void Brewer::Platform::X86::ASMPrinter::Add(Value* value, const Storage& dst)
{
    if (Storage::Equal(dst, m_LastDst))
        ClearLast();
    Asm("add", value, dst);
}

void Brewer::Platform::X86::ASMPrinter::Sub(Value* value, const Storage& dst)
{
    if (Storage::Equal(dst, m_LastDst))
        ClearLast();
    Asm("sub", value, dst);
}

void Brewer::Platform::X86::ASMPrinter::IMul(Value* value, const Storage& dst)
{
    if (Storage::Equal(dst, m_LastDst))
        ClearLast();
    Asm("imul", value, dst);
}

void Brewer::Platform::X86::ASMPrinter::Cmp(Value* value, const Storage& dst)
{
    ClearLast();
    Asm("cmp", value, dst);
}

void Brewer::Platform::X86::ASMPrinter::Lea(Value* value, const Storage& dst)
{
    if (Storage::Equal(dst, m_LastDst))
        ClearLast();
    Asm("lea", value, dst);
}

void Brewer::Platform::X86::ASMPrinter::Ret() const
{
    S() << '\t' << "ret" << std::endl;
}

void Brewer::Platform::X86::ASMPrinter::Call(Value* value, const Storage& dst)
{
    S() << '\t' << "call ";
    PrintCallee(value);
    S() << std::endl;

    if (!dst) return;
    const Storage rax(RA);
    const auto bytes = value->GetType()->GetElementType()->GetResultType()->GetNumBytes();
    Mov(rax, dst, bytes);
}

void Brewer::Platform::X86::ASMPrinter::Jmp(Value* value)
{
    S() << '\t' << "jmp ";
    PrintCallee(value);
    S() << std::endl;
}

void Brewer::Platform::X86::ASMPrinter::Jl(Value* value)
{
    S() << '\t' << "jl ";
    PrintCallee(value);
    S() << std::endl;
}

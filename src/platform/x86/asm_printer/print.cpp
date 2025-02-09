#include <Brewer/Type.hpp>
#include <Brewer/Platform/X86/ASMPrinter.hpp>
#include <Brewer/Value/Constant.hpp>
#include <Brewer/Value/GlobalValue.hpp>

void Brewer::Platform::X86::ASMPrinter::Print(Value *value, const Storage &dst)
{
    if (const auto ptr = dynamic_cast<Assignment *>(value))
        return Print(ptr);
    if (const auto ptr = dynamic_cast<Constant *>(value))
        return Print(ptr, dst);
    if (const auto ptr = dynamic_cast<Instruction *>(value))
        return Print(ptr, dst);
    if (const auto ptr = dynamic_cast<NamedValue *>(value))
        return Print(ptr, dst);

    Error("X86 - Print(Value*) not implemented: {}", value);
}

void Brewer::Platform::X86::ASMPrinter::Print(const Assignment *value)
{
    const auto src = value->GetSrc();
    const auto dst = value->GetDst();
    const auto bytes = src->GetType()->GetNumBytes();
    const auto displacement = GetOffset(dst);

    const Storage rax(RA);
    const Storage storage(0, displacement, RBP, ZERO, 0);
    Print(src, rax);
    Mov(rax, storage, bytes);
}

void Brewer::Platform::X86::ASMPrinter::Print(Constant *value, const Storage &dst)
{
    if (const auto ptr = dynamic_cast<ConstantInt *>(value))
        return Print(ptr, dst);

    Error("X86 - Print(Constant*) not implemented: {}", value);
}

void Brewer::Platform::X86::ASMPrinter::Print(const ConstantInt *value, const Storage &dst)
{
    const auto bytes = value->GetType()->GetNumBytes();
    const Storage imm(value->GetVal());
    Mov(imm, dst, bytes);
}

void Brewer::Platform::X86::ASMPrinter::Print(NamedValue *value, const Storage &dst)
{
    if (const auto ptr = dynamic_cast<FunctionBlock *>(value))
        return Print(ptr, dst);
    if (const auto ptr = dynamic_cast<GlobalValue *>(value))
        return Print(ptr, dst);

    const auto displacement = GetOffset(value);
    const auto bytes = value->GetType()->GetNumBytes();
    const Storage storage(0, displacement, RBP, ZERO, 0);
    Mov(storage, dst, bytes);
}

void Brewer::Platform::X86::ASMPrinter::Print(FunctionBlock *value, const Storage &dst)
{
    Lea(value, dst);
}

void Brewer::Platform::X86::ASMPrinter::Print(GlobalValue *value, const Storage &dst)
{
    Lea(value, dst);
}

#include <Brewer/Platform/X86/ASMPrinter.hpp>
#include <Brewer/Value/GlobalValue.hpp>

void Brewer::Platform::X86::ASMPrinter::PrintCallee(Value *value)
{
    if (const auto ptr = dynamic_cast<NamedValue *>(value))
        return PrintCallee(ptr);

    Error("X86 - PrintCallee(Value*) not implemented: {}", value);
}

void Brewer::Platform::X86::ASMPrinter::PrintCallee(NamedValue *value)
{
    if (const auto ptr = dynamic_cast<GlobalValue *>(value))
        return static_cast<const ASMPrinter *>(this)->PrintCallee(ptr);
    if (const auto ptr = dynamic_cast<FunctionBlock *>(value))
        return static_cast<const ASMPrinter *>(this)->PrintCallee(ptr);

    S() << '*' << GetOffset(value) << "(%rbp)";
}

void Brewer::Platform::X86::ASMPrinter::PrintCallee(const GlobalValue *value) const
{
    S() << value->GetName();
}

void Brewer::Platform::X86::ASMPrinter::PrintCallee(const FunctionBlock *value) const
{
    S() << ".L" << value->GetIndex();
}

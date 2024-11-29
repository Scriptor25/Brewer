#include <Brewer/Platform/X86/ASMPrinter.hpp>
#include <Brewer/Value/GlobalValue.hpp>

void Brewer::Platform::X86::ASMPrinter::PrintCallee(Value* value)
{
    if (const auto ptr = dynamic_cast<NamedValue*>(value))
        return PrintCallee(ptr);

    Error("X86Printer::PrintCallee(Value*) not implemented: {}", value);
}

void Brewer::Platform::X86::ASMPrinter::PrintCallee(NamedValue* value)
{
    if (const auto ptr = dynamic_cast<GlobalValue*>(value))
        return PrintCallee(ptr);
    if (const auto ptr = dynamic_cast<FunctionBlock*>(value))
        return PrintCallee(ptr);

    S() << '*' << GetOffset(value) << "(%rbp)";
}

void Brewer::Platform::X86::ASMPrinter::PrintCallee(GlobalValue* value)
{
    S() << value->GetName();
}

void Brewer::Platform::X86::ASMPrinter::PrintCallee(FunctionBlock* value)
{
    S() << ".L" << value->GetIndex();
}

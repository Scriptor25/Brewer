#include <Brewer/Platform/X86/ASMPrinter.hpp>
#include <Brewer/Value/Constant.hpp>
#include <Brewer/Value/GlobalValue.hpp>

void Brewer::Platform::X86::ASMPrinter::PrintOperand(Value *value)
{
    if (const auto ptr = dynamic_cast<Assignment *>(value))
        return PrintOperand(ptr);
    if (const auto ptr = dynamic_cast<Constant *>(value))
        return PrintOperand(ptr);
    if (const auto ptr = dynamic_cast<NamedValue *>(value))
        return PrintOperand(ptr);

    Error("X86 - PrintOperand(Value*) not implemented: {}", value);
}

void Brewer::Platform::X86::ASMPrinter::PrintOperand(Assignment *value)
{
    Error("X86 - PrintOperand(Assignment*) not implemented: {}", value);
}

void Brewer::Platform::X86::ASMPrinter::PrintOperand(Constant *value)
{
    if (const auto ptr = dynamic_cast<ConstantInt *>(value))
        return PrintOperand(ptr);
    if (const auto ptr = dynamic_cast<ConstantFloat *>(value))
        return PrintOperand(ptr);

    Error("X86 - PrintOperand(Constant*) not implemented: {}", value);
}

void Brewer::Platform::X86::ASMPrinter::PrintOperand(const ConstantInt *value)
{
    S() << '$' << value->GetVal();
}

void Brewer::Platform::X86::ASMPrinter::PrintOperand(ConstantFloat *value)
{
    Error("X86 - PrintOperand(ConstantFloat*) not implemented: {}", value);
}

void Brewer::Platform::X86::ASMPrinter::PrintOperand(NamedValue *value)
{
    if (const auto ptr = dynamic_cast<FunctionBlock *>(value))
        return PrintOperand(ptr);
    if (const auto ptr = dynamic_cast<GlobalValue *>(value))
        return PrintOperand(ptr);

    S() << GetOffset(value) << "(%rbp)";
}

void Brewer::Platform::X86::ASMPrinter::PrintOperand(const FunctionBlock *value)
{
    S() << ".L" << value->GetIndex() << "(%rip)";
}

void Brewer::Platform::X86::ASMPrinter::PrintOperand(const GlobalValue *value)
{
    S() << value->GetName() << "(%rip)";
}

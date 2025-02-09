#include <Brewer/Platform/X86/ASMPrinter.hpp>
#include <Brewer/Value/Constant.hpp>

void Brewer::Platform::X86::ASMPrinter::PrintGlobalOperand(Value *value)
{
    if (const auto ptr = dynamic_cast<Constant *>(value))
        return PrintGlobalOperand(ptr);

    Error("X86 - PrintGlobalOperand(Value*) not implemented: {}", value);
}

void Brewer::Platform::X86::ASMPrinter::PrintGlobalOperand(Constant *value)
{
    if (const auto ptr = dynamic_cast<ConstantInt *>(value))
        return PrintGlobalOperand(ptr);
    if (const auto ptr = dynamic_cast<ConstantArray *>(value))
        return PrintGlobalOperand(ptr);

    Error("X86 - PrintGlobalOperand(Constant*) not implemented: {}", value);
}

void Brewer::Platform::X86::ASMPrinter::PrintGlobalOperand(const ConstantInt *value)
{
    S() << value->GetVal();
}

void Brewer::Platform::X86::ASMPrinter::PrintGlobalOperand(const ConstantArray *value)
{
    for (unsigned i = 0; i < value->GetNumVals(); ++i)
    {
        if (i > 0)
            S() << ", ";
        PrintGlobalOperand(value->GetVal(i));
    }
}

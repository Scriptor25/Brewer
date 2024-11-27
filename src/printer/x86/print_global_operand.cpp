#include <Brewer/Printer/X86Printer.hpp>
#include <Brewer/Value/Constant.hpp>

void Brewer::X86Printer::PrintGlobalOperand(Value* value)
{
    if (const auto ptr = dynamic_cast<Constant*>(value))
        return PrintGlobalOperand(ptr);

    Error("X86Printer::PrintGlobalOperand(Value*) not implemented: {}", value);
}

void Brewer::X86Printer::PrintGlobalOperand(Constant* value)
{
    if (const auto ptr = dynamic_cast<ConstantInt*>(value))
        return PrintGlobalOperand(ptr);
    if (const auto ptr = dynamic_cast<ConstantArray*>(value))
        return PrintGlobalOperand(ptr);

    Error("X86Printer::PrintGlobalOperand(Constant*) not implemented: {}", value);
}

void Brewer::X86Printer::PrintGlobalOperand(ConstantInt* value)
{
    S() << value->GetVal();
}

void Brewer::X86Printer::PrintGlobalOperand(ConstantArray* value)
{
    for (unsigned i = 0; i < value->GetNumVals(); ++i)
    {
        if (i > 0) S() << ", ";
        PrintGlobalOperand(value->GetVal(i));
    }
}

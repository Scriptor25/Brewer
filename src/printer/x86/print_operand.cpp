#include <Brewer/Printer/X86Printer.hpp>
#include <Brewer/Value/Constant.hpp>
#include <Brewer/Value/GlobalValue.hpp>

void Brewer::X86Printer::PrintOperand(Value* value)
{
    if (const auto ptr = dynamic_cast<Assignment*>(value))
        return PrintOperand(ptr);
    if (const auto ptr = dynamic_cast<Constant*>(value))
        return PrintOperand(ptr);
    if (const auto ptr = dynamic_cast<NamedValue*>(value))
        return PrintOperand(ptr);

    Error("X86Printer::PrintOperand(Value*) not implemented: {}", value);
}

void Brewer::X86Printer::PrintOperand(Assignment* value)
{
    Error("X86Printer::PrintOperand(Assignment*) not implemented: {}", value);
}

void Brewer::X86Printer::PrintOperand(Constant* value)
{
    if (const auto ptr = dynamic_cast<ConstantInt*>(value))
        return PrintOperand(ptr);
    if (const auto ptr = dynamic_cast<ConstantFloat*>(value))
        return PrintOperand(ptr);

    Error("X86Printer::PrintOperand(Constant*) not implemented: {}", value);
}

void Brewer::X86Printer::PrintOperand(ConstantInt* value)
{
    S() << '$' << value->GetVal();
}

void Brewer::X86Printer::PrintOperand(ConstantFloat* value)
{
    Error("X86Printer::PrintOperand(ConstantFloat*) not implemented: {}", value);
}

void Brewer::X86Printer::PrintOperand(NamedValue* value)
{
    if (const auto ptr = dynamic_cast<FunctionBlock*>(value))
        return PrintOperand(ptr);
    if (const auto ptr = dynamic_cast<GlobalValue*>(value))
        return PrintOperand(ptr);

    S() << GetOffset(value) << "(%rbp)";
}

void Brewer::X86Printer::PrintOperand(FunctionBlock* value)
{
    S() << ".L" << value->GetIndex() << "(%rip)";
}

void Brewer::X86Printer::PrintOperand(GlobalValue* value)
{
    S() << value->GetName() << "(%rip)";
}

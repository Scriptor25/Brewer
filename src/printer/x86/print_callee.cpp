#include <Brewer/Printer/X86Printer.hpp>
#include <Brewer/Value/GlobalValue.hpp>

void Brewer::X86Printer::PrintCallee(Value* value)
{
    if (const auto ptr = dynamic_cast<NamedValue*>(value))
        return PrintCallee(ptr);

    Error("X86Printer::PrintCallee(Value*) not implemented: {}", value);
}

void Brewer::X86Printer::PrintCallee(NamedValue* value)
{
    if (const auto ptr = dynamic_cast<GlobalValue*>(value))
        return PrintCallee(ptr);
    if (const auto ptr = dynamic_cast<FunctionBlock*>(value))
        return PrintCallee(ptr);

    S() << '*' << GetOffset(value) << "(%rbp)";
}

void Brewer::X86Printer::PrintCallee(GlobalValue* value)
{
    S() << value->GetName();
}

void Brewer::X86Printer::PrintCallee(FunctionBlock* value)
{
    S() << ".L" << value->GetIndex();
}

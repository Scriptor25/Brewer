#include <ostream>
#include <Brewer/Module.hpp>
#include <Brewer/Printer/IRPrinter.hpp>
#include <Brewer/Value/GlobalValue.hpp>
#include <Brewer/Value/Value.hpp>

Brewer::IRPrinter::IRPrinter(std::ostream& stream)
    : Printer(stream)
{
}

void Brewer::IRPrinter::Print(Module* module)
{
    module->ForEach([&](GlobalValue* value)
    {
        Print(value);
        S() << std::endl;
    });
}

void Brewer::IRPrinter::Print(Value* value)
{
    value->PrintIR(S());
}

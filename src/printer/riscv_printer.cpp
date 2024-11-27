#include <Brewer/Module.hpp>
#include <Brewer/Printer/RiscVPrinter.hpp>
#include <Brewer/Value/GlobalValue.hpp>

#include "Brewer/Printer/X86Printer.hpp"

Brewer::RiscVPrinter::RiscVPrinter(std::ostream& stream)
    : Printer(stream)
{
}

void Brewer::RiscVPrinter::Print(Module* module)
{
    module->ForEach([&](GlobalValue* value)
    {
        Print(value);
    });
}

void Brewer::RiscVPrinter::Print(Value* value)
{
    Error("RiscVPrinter::Print not implemented: {}", value);
}

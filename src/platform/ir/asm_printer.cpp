#include <ostream>
#include <Brewer/Module.hpp>
#include <Brewer/Platform/IR/ASMPrinter.hpp>
#include <Brewer/Value/GlobalValue.hpp>
#include <Brewer/Value/Value.hpp>

Brewer::Platform::IR::ASMPrinter::ASMPrinter(std::ostream &stream)
    : ASMPrinterBase(stream)
{
}

void Brewer::Platform::IR::ASMPrinter::Print(Module &module)
{
    module.ForEach(
        [&](GlobalValue *value)
        {
            Print(value);
            S() << std::endl;
        });
}

void Brewer::Platform::IR::ASMPrinter::Print(Value *value)
{
    value->PrintIR(S());
}

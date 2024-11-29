#pragma once

#include <Brewer/Printer.hpp>

namespace Brewer::Platform::RiscV
{
    class ASMPrinter : public ASMPrinterBase
    {
    public:
        explicit ASMPrinter(std::ostream& stream);

        void Print(Module& module) override;
        void Print(Value* value) override;
    };
}

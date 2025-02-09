#pragma once

#include <Brewer/Printer.hpp>

namespace Brewer::Platform::IR
{
    class ASMPrinter final : public ASMPrinterBase
    {
    public:
        explicit ASMPrinter(std::ostream &stream);

        void Print(Module &module) override;
        void Print(Value *value) override;
    };
}

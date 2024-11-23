#pragma once

#include <Brewer/Printer/Printer.hpp>

namespace Brewer
{
    class RiscVPrinter : public Printer
    {
    public:
        explicit RiscVPrinter(std::ostream& stream);

        void Print(Module* module) override;
        void Print(Value* value) override;
    };
}

#pragma once

#include <Brewer/Printer/Printer.hpp>

namespace Brewer
{
    class IRPrinter : public Printer
    {
    public:
        explicit IRPrinter(std::ostream& stream);

        void Print(Module* module) override;
        void Print(Value* value) override;
    };
}

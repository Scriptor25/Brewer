#pragma once

#include <Brewer/Brewer.hpp>

namespace Brewer
{
    class ASMPrinterBase
    {
    public:
        explicit ASMPrinterBase(std::ostream& stream);
        virtual ~ASMPrinterBase() = default;

        virtual void Print(Module& module) = 0;
        virtual void Print(Value* value) = 0;

    protected:
        [[nodiscard]] std::ostream& S() const;

    private:
        std::ostream& m_Stream;
    };
}

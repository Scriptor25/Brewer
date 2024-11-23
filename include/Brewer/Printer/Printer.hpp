#pragma once

#include <Brewer/Brewer.hpp>

namespace Brewer
{
    class Printer
    {
    public:
        explicit Printer(std::ostream& stream);
        virtual ~Printer() = default;

        virtual void Print(Module* module) = 0;
        virtual void Print(Value* value) = 0;

    protected:
        [[nodiscard]] std::ostream& S() const;

    private:
        std::ostream& m_Stream;
    };
}

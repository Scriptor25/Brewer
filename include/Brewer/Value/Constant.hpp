#pragma once

#include <cstdint>
#include <vector>
#include <Brewer/Value/Value.hpp>

namespace Brewer
{
    class Constant : public Value
    {
    public:
        explicit Constant(Type* type);

        std::ostream& Print(std::ostream& os) const override;
    };

    class ConstantInt : public Constant
    {
    public:
        ConstantInt(IntType* type, uint64_t val);

        std::ostream& PrintOperand(std::ostream& os) const override;

    private:
        uint64_t m_Val;
    };

    class ConstantFloat : public Constant
    {
    public:
        ConstantFloat(FloatType* type, double val);

        std::ostream& PrintOperand(std::ostream& os) const override;

    private:
        double m_Val;
    };

    class ConstantArray : public Constant
    {
    public:
        ConstantArray(ArrayType* type, std::vector<Constant*> elements);

        std::ostream& PrintOperand(std::ostream& os) const override;

    private:
        std::vector<Constant*> m_Elements;
    };
}

#pragma once

#include <cstdint>
#include <Brewer/Value/Value.hpp>

namespace Brewer
{
    class Constant : public Value
    {
    public:
        explicit Constant(Type* type);

        [[nodiscard]] bool RequiresDestination() const override;
    };

    class ConstantInt : public Constant
    {
    public:
        ConstantInt(IntType* type, uint64_t val);

        [[nodiscard]] uint64_t GetVal() const;

        [[nodiscard]] bool NotNull() const override;
        std::ostream& PrintOperandIR(std::ostream& os, bool omit_type) const override;

    private:
        uint64_t m_Val;
    };

    class ConstantFloat : public Constant
    {
    public:
        ConstantFloat(FloatType* type, double val);

        [[nodiscard]] double GetVal() const;

        [[nodiscard]] bool NotNull() const override;
        std::ostream& PrintOperandIR(std::ostream& os, bool omit_type) const override;

    private:
        double m_Val;
    };

    class ConstantArray : public Constant
    {
    public:
        ConstantArray(ArrayType* type, std::vector<Constant*> vals);

        [[nodiscard]] Constant* GetVal(unsigned i) const;
        [[nodiscard]] unsigned GetNumVals() const;

        std::ostream& PrintOperandIR(std::ostream& os, bool omit_type) const override;

    private:
        std::vector<Constant*> m_Vals;
    };
}

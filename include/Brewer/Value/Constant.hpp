#pragma once

#include <cstdint>
#include <Brewer/Value/Value.hpp>

namespace Brewer
{
    class Constant : public Value
    {
    public:
        Constant(Type* type);
    };

    class ConstantInt : public Constant
    {
    public:
        ConstantInt(IntType* type, uint64_t val);

        uint64_t GetVal() const;

        std::ostream& PrintOperandIR(std::ostream& os, bool omit_type) const override;

    private:
        uint64_t m_Val;
    };

    class ConstantFloat : public Constant
    {
    public:
        ConstantFloat(FloatType* type, double val);

        double GetVal() const;

        std::ostream& PrintOperandIR(std::ostream& os, bool omit_type) const override;

    private:
        double m_Val;
    };

    class ConstantArray : public Constant
    {
    public:
        ConstantArray(ArrayType* type, std::vector<Constant*> vals);

        Constant* GetVal(unsigned i) const;
        unsigned GetNumVals() const;

        std::ostream& PrintOperandIR(std::ostream& os, bool omit_type) const override;

    private:
        std::vector<Constant*> m_Vals;
    };
}

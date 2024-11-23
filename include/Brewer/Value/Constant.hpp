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

        std::ostream& PrintIR(std::ostream& os) const override;
    };

    class ConstantInt : public Constant
    {
    public:
        ConstantInt(IntType* type, uint64_t val);

        std::ostream& PrintIROperand(std::ostream& os) const override;

        [[nodiscard]] uint64_t GetVal() const;

    private:
        uint64_t m_Val;
    };

    class ConstantFloat : public Constant
    {
    public:
        ConstantFloat(FloatType* type, double val);

        std::ostream& PrintIROperand(std::ostream& os) const override;

        [[nodiscard]] double GetVal() const;

    private:
        double m_Val;
    };

    class ConstantArray : public Constant
    {
    public:
        ConstantArray(ArrayType* type, std::vector<Constant*> elements);

        std::ostream& PrintIROperand(std::ostream& os) const override;

        template <typename T = Constant>
        [[nodiscard]] T* GetElement(const unsigned i) const
        {
            return dynamic_cast<T*>(m_Elements[i]);
        }

        [[nodiscard]] unsigned GetNumElements() const;

    private:
        std::vector<Constant*> m_Elements;
    };
}

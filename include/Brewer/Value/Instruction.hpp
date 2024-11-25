#pragma once

#include <vector>
#include <Brewer/Value/NamedValue.hpp>

namespace Brewer
{
    class Instruction : public NamedValue
    {
    public:
        enum Code
        {
            IAdd,
            ISub,
            Call,
            GEP,
            PHI,
            Ret,
            Br,
            Br_LT,
        };

        Instruction(Type* type, std::string name, Code code, std::vector<Value*> operands);

        std::ostream& PrintIR(std::ostream& os) const override;

        void ReplaceUseOf(Value* old_value, Value* new_value) override;

        [[nodiscard]] Code GetCode() const;
        [[nodiscard]] Value* GetOperand(unsigned i) const;
        [[nodiscard]] unsigned GetNumOperands() const;
        [[nodiscard]] std::vector<Value*> GetSubOperands(unsigned begin, unsigned end = 0) const;

    private:
        Code m_Code;
        std::vector<Value*> m_Operands;
    };

    Instruction::Code ToCode(const std::string& name);
    std::string ToString(Instruction::Code code);

    std::ostream& operator<<(std::ostream& os, Instruction::Code code);
}

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
            ICmpLE,
            ISub,
            IAdd,
            Call,
            GEP,
            PHI,
            Ret,
            Br,
        };

        Instruction(Type* type, std::string name, Code code, std::vector<Value*> operands);

        std::ostream& Print(std::ostream& os) const override;

        void ReplaceUseOf(Value* old_value, Value* new_value) override;

    private:
        Code m_Code;
        std::vector<Value*> m_Operands;
    };

    Instruction::Code ToCode(const std::string& name);
    std::string ToString(Instruction::Code code);

    std::ostream& operator<<(std::ostream& os, Instruction::Code code);
}

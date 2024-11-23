#pragma once

#include <Brewer/Value/NamedValue.hpp>

namespace Brewer
{
    class Block : public NamedValue
    {
    public:
        Block(BlockType* type, std::string name);

        std::ostream& PrintIR(std::ostream& os) const override;
        std::ostream& PrintIROperand(std::ostream& os) const override;

        NamedValue* GetValue(Type* type, const std::string& name) const;
        void Append(Value* value);

        [[nodiscard]] Value* GetValue(unsigned i) const;
        [[nodiscard]] unsigned GetNumValues() const;

    private:
        std::vector<Value*> m_Values;
    };
}

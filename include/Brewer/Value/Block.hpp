#pragma once

#include <Brewer/Value/NamedValue.hpp>

namespace Brewer
{
    class Block : public NamedValue
    {
    public:
        Block(BlockType* type, std::string name);

        std::ostream& Print(std::ostream& os) const override;
        std::ostream& PrintOperand(std::ostream& os) const override;

        void ReplaceUseOf(Value* old_value, Value* new_value) override;

        NamedValue* GetValue(const Type* type, const std::string& name);
        void Append(Value* value);

    private:
        std::vector<Value*> m_Values;
    };
}

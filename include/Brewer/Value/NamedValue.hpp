#pragma once

#include <Brewer/Value/Value.hpp>

namespace Brewer
{
    class NamedValue : public Value
    {
    public:
        NamedValue(Type* type, std::string name);

        std::ostream& PrintOperandIR(std::ostream& os, bool omit_type) const override;

        [[nodiscard]] std::string GetName() const;

    private:
        std::string m_Name;
    };

    class FunctionArg : public NamedValue
    {
    public:
        FunctionArg(Type* type, std::string name);
    };

    class FunctionBlock : public NamedValue
    {
    public:
        FunctionBlock(BlockType* type, std::string name);

        [[nodiscard]] Value* GetValue(unsigned i) const;
        [[nodiscard]] unsigned GetNumValues() const;

        void Append(Value* value);
        NamedValue* Get(Type* type, const std::string& name) const;

        void Replace(Value* old_value, Value* new_value) override;

    private:
        std::vector<Value*> m_Values;
    };
}

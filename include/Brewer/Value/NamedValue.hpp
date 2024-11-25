#pragma once

#include <Brewer/Value/Value.hpp>

namespace Brewer
{
    class NamedValue : public Value
    {
    public:
        NamedValue(Type* type, std::string name);

        std::ostream& PrintOperandIR(std::ostream& os, bool omit_type) const override;

        std::string GetName() const;

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

        Value* GetValue(unsigned i) const;
        unsigned GetNumValues() const;

        void Append(Value* value);
        NamedValue* Get(Type* type, const std::string& name) const;
    };
}

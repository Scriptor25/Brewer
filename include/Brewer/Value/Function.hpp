#pragma once

#include <Brewer/Value/GlobalValue.hpp>

namespace Brewer
{
    class Function : public GlobalValue
    {
    public:
        Function(
            FunctionType* function_type,
            std::string name,
            LinkageType linkage,
            std::vector<Argument*> args);

        std::ostream& PrintIR(std::ostream& os) const override;
        void ReplaceUseOf(Value* old_value, Value* new_value) override;

        NamedValue* GetValue(Type* type, const std::string& name);
        void Append(Value* value);

        [[nodiscard]] bool IsEmpty() const;

        [[nodiscard]] Argument* GetArgument(unsigned i) const;
        [[nodiscard]] unsigned GetNumArguments() const;
        [[nodiscard]] Block* GetBlock(unsigned i) const;
        [[nodiscard]] unsigned GetNumBlocks() const;

        [[nodiscard]] unsigned CountArgBytes() const;
        [[nodiscard]] unsigned CountNamedValueBytes() const;

    private:
        std::vector<Argument*> m_Args;
        std::vector<Block*> m_Blocks;
        std::vector<NamedValue*> m_PreUses;
    };
}

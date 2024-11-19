#pragma once

#include <Brewer/Value/GlobalValue.hpp>

namespace Brewer
{
    class Function : public GlobalValue
    {
    public:
        Function(FunctionType* function_type, std::string name, LinkageType linkage, std::vector<Argument*> args);

        std::ostream& Print(std::ostream& os) const override;

        void ReplaceUseOf(Value* old_value, Value* new_value) override;

        Block* GetBlock(BlockType* type, const std::string& name);
        NamedValue* GetValue(Type* type, const std::string& name);

        void Append(Value* value);

    private:
        std::vector<Argument*> m_Args;
        std::vector<Block*> m_Blocks;
        std::vector<NamedValue*> m_PreUses;
        Block* m_InsertBlock{};
    };
}

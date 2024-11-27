#pragma once

#include <Brewer/Value/NamedValue.hpp>

namespace Brewer
{
    class GlobalValue : public NamedValue
    {
    public:
        enum Linkage
        {
            Linkage_Local,
            Linkage_Weak,
            Linkage_Global,
        };

        GlobalValue(Type* type, std::string name, Linkage linkage);

        std::ostream& PrintOperandIR(std::ostream& os, bool omit_type) const override;

        [[nodiscard]] Linkage GetLinkage() const;

    private:
        Linkage m_Linkage;
    };

    class GlobalVariable : public GlobalValue
    {
    public:
        GlobalVariable(Type* type, std::string name, Linkage linkage, Constant* init);

        [[nodiscard]] Constant* GetInit() const;

        void Replace(Value* old_value, Value* new_value) override;

    private:
        Constant* m_Init;
    };

    class GlobalFunction : public GlobalValue
    {
    public:
        GlobalFunction(FunctionType* type, std::string name, Linkage linkage, std::vector<FunctionArg*> args);

        [[nodiscard]] FunctionArg* GetArg(unsigned i) const;
        [[nodiscard]] unsigned GetNumArgs() const;
        [[nodiscard]] FunctionBlock* GetBlock(unsigned i) const;
        [[nodiscard]] unsigned GetNumBlocks() const;

        [[nodiscard]] bool IsEmpty() const;
        [[nodiscard]] unsigned GetByteAlloc() const;

        void Append(Value* value);

        NamedValue* Get(Type* type, const std::string& name);

        void Replace(Value* old_value, Value* new_value) override;

    private:
        std::vector<FunctionArg*> m_Args;
        std::vector<NamedValue*> m_Locals;
        std::vector<FunctionBlock*> m_Blocks;
        std::vector<FunctionBlock*> m_Unresolved;
    };

    GlobalValue::Linkage ToLinkage(const std::string& str);
}

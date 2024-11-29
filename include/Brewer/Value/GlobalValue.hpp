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
        [[nodiscard]] Linkage GetLinkage() const;

        bool NeedsDestination() const override;
        std::ostream& PrintOperandIR(std::ostream& os, bool omit_type) const override;

    private:
        Linkage m_Linkage;
    };

    class GlobalVariable : public GlobalValue
    {
    public:
        GlobalVariable(Type* type, std::string name, Linkage linkage, Constant* init);
        [[nodiscard]] Constant* GetInit() const;

        std::ostream& PrintIR(std::ostream& os) const override;

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
        void Append(Value* value);
        void EraseBlock(unsigned i);
        bool HasUnresolved() const;
        NamedValue* Get(Type* type, const std::string& name);

        [[nodiscard]] uint64_t CountAlloca() const override;
        std::ostream& PrintIR(std::ostream& os) const override;

    private:
        std::vector<FunctionArg*> m_Args;
        std::vector<NamedValue*> m_Locals;
        std::vector<FunctionBlock*> m_Blocks;
        std::vector<FunctionBlock*> m_Unresolved;
    };

    GlobalValue::Linkage ToLinkage(const std::string& str);
    std::string ToString(GlobalValue::Linkage linkage);
}

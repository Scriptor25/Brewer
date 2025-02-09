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

        GlobalValue(Type *type, std::string name, Linkage linkage, std::vector<std::string> &&meta);

        [[nodiscard]] Linkage GetLinkage() const;

        [[nodiscard]] bool RequiresDestination() const override;
        std::ostream &PrintOperandIR(std::ostream &os, bool omit_type) const override;

    private:
        Linkage m_Linkage;
    };

    class GlobalVariable final : public GlobalValue
    {
    public:
        GlobalVariable(Type *type, std::string name, Linkage linkage, Constant *init, std::vector<std::string> &&meta);

        [[nodiscard]] Constant *GetInit() const;

        std::ostream &PrintIR(std::ostream &os) const override;

    private:
        Constant *m_Init;
    };

    class GlobalFunction final : public GlobalValue
    {
    public:
        GlobalFunction(
            FunctionType *type,
            std::string name,
            Linkage linkage,
            std::vector<FunctionArg *> args,
            std::vector<std::string> &&meta);

        [[nodiscard]] FunctionArg *GetArg(unsigned i) const;
        [[nodiscard]] unsigned GetNumArgs() const;
        [[nodiscard]] FunctionBlock *GetBlock(unsigned i) const;
        [[nodiscard]] unsigned GetNumBlocks() const;
        [[nodiscard]] bool IsEmpty() const;
        [[nodiscard]] bool HasUnresolved() const;

        void Append(Value *value);
        void EraseBlock(unsigned i);
        NamedValue *Get(Type *type, const std::string &name);

        [[nodiscard]] uint64_t GetNumAllocaBytes() const override;
        std::ostream &PrintIR(std::ostream &os) const override;

    private:
        std::vector<FunctionArg *> m_Args;
        std::vector<NamedValue *> m_Locals;
        std::vector<FunctionBlock *> m_Blocks;
        std::vector<FunctionBlock *> m_Unresolved;
    };

    GlobalValue::Linkage ToLinkage(const std::string &str);
    std::string ToString(GlobalValue::Linkage linkage);
}

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

        Linkage GetLinkage() const;

    private:
        Linkage m_Linkage;
    };

    class GlobalVariable : public GlobalValue
    {
    public:
        GlobalVariable(Type* type, std::string name, Linkage linkage, Constant* init);

        Constant* GetInit() const;

    private:
        Constant* m_Init;
    };

    class GlobalFunction : public GlobalValue
    {
    public:
        GlobalFunction(FunctionType* type, std::string name, Linkage linkage, std::vector<FunctionArg*> args);

        FunctionArg* GetArg(unsigned i) const;
        unsigned GetNumArgs() const;
        FunctionBlock* GetBlock(unsigned i) const;
        unsigned GetNumBlocks() const;

        bool IsEmpty() const;
        unsigned GetByteAlloc() const;

        void Append(Value* value);

        NamedValue* Get(Type* type, const std::string& name);

    private:
        std::vector<FunctionArg*> m_Args;
        std::vector<FunctionBlock*> m_Blocks;

        std::vector<NamedValue*> m_Unresolved;
    };

    GlobalValue::Linkage ToLinkage(const std::string& str);
}

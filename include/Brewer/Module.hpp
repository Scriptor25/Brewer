#pragma once

#include <map>
#include <string>
#include <Brewer/Brewer.hpp>

namespace Brewer
{
    class Module
    {
    public:
        explicit Module(Context& context);

        [[nodiscard]] Context& GetContext() const;

        GlobalValue* GetSymbol(const std::string& name);
        GlobalValue* AddSymbol(const std::string& name, GlobalValue* value);

    private:
        Context& m_Context;
        std::map<std::string, GlobalValue*> m_SymbolTable;
    };
}

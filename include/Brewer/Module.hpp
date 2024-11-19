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

        std::ostream& Print(std::ostream& os) const;

        GlobalValue* GetGlobalValue(Type* type, const std::string& name);
        GlobalValue* SetGlobalValue(const std::string& name, GlobalValue* value);

    private:
        Context& m_Context;
        std::map<std::string, GlobalValue*> m_SymbolTable;
    };
}

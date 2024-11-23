#pragma once

#include <functional>
#include <string>
#include <vector>
#include <Brewer/Brewer.hpp>

namespace Brewer
{
    class Module
    {
    public:
        explicit Module(Context& context);

        [[nodiscard]] Context& GetContext() const;

        void Print(Printer* printer);
        std::ostream& PrintIR(std::ostream& os) const;

        GlobalValue* GetGlobalValue(Type* type, const std::string& name);
        void SetGlobalValue(const std::string& name, GlobalValue* new_value);

        void ForEach(const std::function<void(GlobalValue*)>& consumer) const;

    private:
        Context& m_Context;

        std::vector<GlobalValue*> m_SymbolTable;
    };
}

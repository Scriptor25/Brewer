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
        Module(Context& context, const std::string& filename);

        [[nodiscard]] Context& GetContext() const;
        std::string GetFilename() const;

        void Print(Printer* printer);
        std::ostream& PrintIR(std::ostream& os) const;

        void Append(GlobalValue* value);
        GlobalValue* Get(PointerType* type, const std::string& name);

        void ForEach(const std::function<void(GlobalValue*)>& consumer) const;

    private:
        Context& m_Context;
        std::string m_Filename;
        std::vector<GlobalValue*> m_SymbolTable;
    };
}

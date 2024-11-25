#include <Brewer/Module.hpp>
#include <Brewer/Type.hpp>
#include <Brewer/Printer/Printer.hpp>
#include <Brewer/Value/GlobalValue.hpp>

Brewer::Module::Module(Context& context)
    : m_Context(context)
{
}

Brewer::Context& Brewer::Module::GetContext() const
{
    return m_Context;
}

void Brewer::Module::Print(Printer* printer)
{
    printer->Print(this);
}

std::ostream& Brewer::Module::PrintIR(std::ostream& os) const
{
    bool first = true;
    for (const auto& global : m_SymbolTable)
    {
        if (first)
            first = false;
        else os << std::endl;
        global->PrintIR(os) << std::endl;
    }
    return os;
}

Brewer::GlobalValue* Brewer::Module::GetGlobalValue(Type* type, const std::string& name)
{
    if (const auto global = Find(m_SymbolTable, type, name))
        return global;

    const auto global = new GlobalValue(dynamic_cast<PointerType*>(type)->GetElementType(), name, GlobalValue::NoLinkage);
    m_SymbolTable.push_back(global);
    return global;
}

void Brewer::Module::SetGlobalValue(const std::string& name, GlobalValue* new_value)
{
    if (const auto old_value = Find(m_SymbolTable, name))
    {
        Replace(m_SymbolTable, old_value, new_value);
        old_value->ReplaceAllUses(new_value);
        return;
    }

    m_SymbolTable.push_back(new_value);
}

void Brewer::Module::ForEach(const std::function<void(GlobalValue*)>& consumer) const
{
    for (const auto& global : m_SymbolTable)
        consumer(global);
}

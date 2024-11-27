#include <Brewer/Module.hpp>
#include <Brewer/Type.hpp>
#include <Brewer/Printer/Printer.hpp>
#include <Brewer/Value/GlobalValue.hpp>
#include <utility>

Brewer::Module::Module(Context& context, std::string filename)
    : m_Context(context), m_Filename(std::move(filename))
{
}

Brewer::Context& Brewer::Module::GetContext() const
{
    return m_Context;
}

std::string Brewer::Module::GetFilename() const
{
    return m_Filename;
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

void Brewer::Module::Append(GlobalValue* value)
{
    if (const auto old_value = Find(m_SymbolTable, value->GetName()))
    {
        Replace(m_SymbolTable, old_value, value);
        old_value->ReplaceWith(value);
        return;
    }

    m_SymbolTable.push_back(value);
}

Brewer::GlobalValue* Brewer::Module::Get(PointerType* type, const std::string& name)
{
    if (const auto global = Find(m_SymbolTable, type, name))
        return global;

    const auto global = new GlobalValue(type->GetElementType(), name, GlobalValue::Linkage_Local);
    m_SymbolTable.push_back(global);
    return global;
}

void Brewer::Module::ForEach(const std::function<void(GlobalValue*)>& consumer) const
{
    for (const auto& global : m_SymbolTable)
        consumer(global);
}

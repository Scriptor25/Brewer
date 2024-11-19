#include <Brewer/Module.hpp>
#include <Brewer/Type.hpp>
#include <Brewer/Value/GlobalValue.hpp>

Brewer::Module::Module(Context& context)
    : m_Context(context)
{
}

Brewer::Context& Brewer::Module::GetContext() const
{
    return m_Context;
}

std::ostream& Brewer::Module::Print(std::ostream& os) const
{
    bool first = true;
    for (const auto& [name, global] : m_SymbolTable)
    {
        if (first)
            first = false;
        else os << std::endl;
        global->Print(os) << std::endl;
    }
    return os;
}

Brewer::GlobalValue* Brewer::Module::GetGlobalValue(Type* type, const std::string& name)
{
    auto& global = m_SymbolTable[name];
    if (global)
    {
        if (global->GetType() != type)
            Error("type mismatch: {} != {}", global->GetType(), type);
        return global;
    }
    const auto pointer_type = dynamic_cast<PointerType*>(type);
    const auto element_type = pointer_type->ElementType();
    return global = new GlobalValue(element_type, name, GlobalValue::NoLinkage);
}

Brewer::GlobalValue* Brewer::Module::SetGlobalValue(const std::string& name, GlobalValue* value)
{
    auto& global = m_SymbolTable[name];
    if (global) global->ReplaceAllUses(value);
    return global = value;
}

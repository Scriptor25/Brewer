#include <Brewer/Module.hpp>

Brewer::Module::Module(Context& context)
    : m_Context(context)
{
}

Brewer::Context& Brewer::Module::GetContext() const
{
    return m_Context;
}

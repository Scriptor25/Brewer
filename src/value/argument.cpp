#include <utility>
#include <Brewer/Type.hpp>
#include <Brewer/Value/Argument.hpp>

Brewer::Argument::Argument(Type* type, std::string name, const unsigned index)
    : NamedValue(type, std::move(name)), m_Index(index)
{
}

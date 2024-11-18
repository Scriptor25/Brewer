#include <utility>
#include <Brewer/Value/Argument.hpp>

Brewer::Argument::Argument(Type* type, std::string name, const unsigned index)
    : Value(type, std::move(name)), m_Index(index)
{
}

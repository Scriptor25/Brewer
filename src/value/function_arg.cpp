#include <Brewer/Value/NamedValue.hpp>

Brewer::FunctionArg::FunctionArg(Type* type, std::string name)
    : NamedValue(type, std::move(name))
{
}

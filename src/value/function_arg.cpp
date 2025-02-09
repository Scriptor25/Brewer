#include <Brewer/Value/NamedValue.hpp>

Brewer::FunctionArg::FunctionArg(Type *type, std::string name, std::vector<std::string> &&meta)
    : NamedValue(type, std::move(name), std::move(meta))
{
}

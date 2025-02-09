#include <Brewer/Value/Constant.hpp>

Brewer::Constant::Constant(Type *type, std::vector<std::string> &&meta)
    : Value(type, std::move(meta))
{
}

bool Brewer::Constant::RequiresDestination() const
{
    return true;
}

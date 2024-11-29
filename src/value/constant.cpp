#include <Brewer/Value/Constant.hpp>

Brewer::Constant::Constant(Type* type)
    : Value(type)
{
}

bool Brewer::Constant::RequiresDestination() const
{
    return true;
}

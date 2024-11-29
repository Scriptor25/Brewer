#include <Brewer/Value/Constant.hpp>

Brewer::Constant::Constant(Type* type)
    : Value(type)
{
}

bool Brewer::Constant::NeedsDestination() const
{
    return true;
}

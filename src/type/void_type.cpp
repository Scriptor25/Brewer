#include <Brewer/Type.hpp>

unsigned Brewer::VoidType::Hash()
{
    return 0x0;
}

Brewer::VoidType::VoidType(Context& context, const unsigned hash)
    : Type(context, hash)
{
}

std::ostream& Brewer::VoidType::Print(std::ostream& os) const
{
    return os << "void";
}

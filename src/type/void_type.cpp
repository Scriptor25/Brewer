#include <Brewer/Type.hpp>

unsigned Brewer::VoidType::Hash()
{
    return 0x1;
}

Brewer::VoidType::VoidType(const unsigned hash)
    : Type(hash)
{
}

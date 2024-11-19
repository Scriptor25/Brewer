#include <Brewer/Type.hpp>

unsigned Brewer::BlockType::Hash()
{
    return 0x1;
}

Brewer::BlockType::BlockType(Context& context, unsigned hash)
    : Type(context, hash)
{
}

std::ostream& Brewer::BlockType::Print(std::ostream& os) const
{
    return os;
}

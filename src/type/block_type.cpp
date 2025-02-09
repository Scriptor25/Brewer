#include <Brewer/Type.hpp>

unsigned Brewer::BlockType::Hash()
{
    return 0x1;
}

Brewer::BlockType::BlockType(Context &context, const unsigned hash)
    : Type(context, hash)
{
}

std::ostream &Brewer::BlockType::Print(std::ostream &os) const
{
    return os;
}

unsigned Brewer::BlockType::GetNumBytes() const
{
    return 0;
}

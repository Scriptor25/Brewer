#include <Brewer/Type.hpp>

unsigned Brewer::Type::CombineHash(const unsigned h1, const unsigned h2)
{
    return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
}

Brewer::Type::Type(const unsigned hash)
    : m_Hash(hash)
{
}

unsigned Brewer::Type::Hash() const
{
    return m_Hash;
}

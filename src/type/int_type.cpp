#include <Brewer/Type.hpp>

unsigned Brewer::IntType::Hash(const unsigned bits)
{
    return std::hash<unsigned>{}(bits) ^ 0x02;
}

Brewer::IntType::IntType(const unsigned hash, const unsigned bits)
    : Type(hash), m_Bits(bits)
{
}

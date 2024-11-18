#include <Brewer/Type.hpp>

unsigned Brewer::FloatType::Hash(const unsigned bits)
{
    return std::hash<unsigned>{}(bits) ^ 0x03;
}

Brewer::FloatType::FloatType(const unsigned hash, const unsigned bits)
    : Type(hash), m_Bits(bits)
{
}

#include <Brewer/Type.hpp>

unsigned Brewer::FloatType::Hash(const unsigned bits)
{
    return std::hash<unsigned>{}(bits) ^ 0x03;
}

Brewer::FloatType::FloatType(Context& context, const unsigned hash, const unsigned bits)
    : Type(context, hash), m_Bits(bits)
{
}

std::ostream& Brewer::FloatType::Print(std::ostream& os) const
{
    return os << 'f' << m_Bits;
}

#include <Brewer/Type.hpp>

unsigned Brewer::IntType::Hash(const unsigned bits)
{
    return std::hash<unsigned>{}(bits) ^ 0x02;
}

Brewer::IntType::IntType(Context &context, const unsigned hash, const unsigned bits)
    : Type(context, hash),
      m_Bits(bits)
{
}

std::ostream &Brewer::IntType::Print(std::ostream &os) const
{
    return os << 'i' << m_Bits;
}

unsigned Brewer::IntType::GetNumBytes() const
{
    return m_Bits / 8;
}

unsigned Brewer::IntType::GetBits() const
{
    return m_Bits;
}

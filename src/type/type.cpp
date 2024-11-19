#include <Brewer/Type.hpp>

unsigned Brewer::Type::CombineHash(const unsigned h1, const unsigned h2)
{
    return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
}

Brewer::Type::Type(Context& context, const unsigned hash)
    : m_Context(context), m_Hash(hash)
{
}

Brewer::Context& Brewer::Type::GetContext() const
{
    return m_Context;
}

unsigned Brewer::Type::Hash() const
{
    return m_Hash;
}

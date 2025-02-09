#include <Brewer/Type.hpp>

unsigned Brewer::Type::CombineHash(const unsigned h1, const unsigned h2)
{
    return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
}

Brewer::Type::Type(Context &context, const unsigned hash)
    : m_Context(context),
      m_Hash(hash)
{
}

Brewer::Context &Brewer::Type::GetContext() const
{
    return m_Context;
}

unsigned Brewer::Type::GetHash() const
{
    return m_Hash;
}

Brewer::Type *Brewer::Type::GetBaseType()
{
    if (const auto element = GetElementType())
        return element->GetBaseType();
    return this;
}

unsigned Brewer::Type::GetBits() const
{
    return 0;
}

Brewer::Type *Brewer::Type::GetElementType() const
{
    return {};
}

Brewer::Type *Brewer::Type::GetElementType(unsigned) const
{
    return {};
}

unsigned Brewer::Type::GetNumElements() const
{
    return 0;
}

Brewer::Type *Brewer::Type::GetResultType() const
{
    return {};
}

Brewer::Type *Brewer::Type::GetArgType(unsigned) const
{
    return {};
}

unsigned Brewer::Type::GetNumArgs() const
{
    return 0;
}

bool Brewer::Type::IsVarArg() const
{
    return false;
}

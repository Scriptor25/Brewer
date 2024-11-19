#include <Brewer/Type.hpp>

unsigned Brewer::ArrayType::Hash(const Type* element_type, const unsigned size)
{
    const auto element_hash = element_type->Hash();
    const auto size_hash = std::hash<unsigned>{}(size);
    return CombineHash(CombineHash(element_hash, size_hash), 0x06);
}

Brewer::ArrayType::ArrayType(Context& context, const unsigned hash, Type* element_type, const unsigned size)
    : Type(context, hash), m_ElementType(element_type), m_Size(size)
{
}

std::ostream& Brewer::ArrayType::Print(std::ostream& os) const
{
    return m_ElementType->Print(os << '[') << " x " << m_Size << ']';
}

Brewer::Type* Brewer::ArrayType::ElementType() const
{
    return m_ElementType;
}

unsigned Brewer::ArrayType::Size() const
{
    return m_Size;
}

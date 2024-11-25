#include <Brewer/Type.hpp>

unsigned Brewer::ArrayType::Hash(const Type* element_type, const unsigned num_elements)
{
    const auto element_hash = element_type->GetHash();
    const auto size_hash = std::hash<unsigned>{}(num_elements);
    return CombineHash(CombineHash(element_hash, size_hash), 0x06);
}

Brewer::ArrayType::ArrayType(Context& context, const unsigned hash, Type* element_type, const unsigned num_elements)
    : Type(context, hash), m_ElementType(element_type), m_NumElements(num_elements)
{
}

std::ostream& Brewer::ArrayType::Print(std::ostream& os) const
{
    return m_ElementType->Print(os << '[') << " x " << m_NumElements << ']';
}

unsigned Brewer::ArrayType::CountBytes() const
{
    return m_NumElements * m_ElementType->CountBytes();
}

unsigned Brewer::ArrayType::GetNumElements() const
{
    return m_NumElements;
}

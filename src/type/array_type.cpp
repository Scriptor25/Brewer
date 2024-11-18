#include <Brewer/Type.hpp>

unsigned Brewer::ArrayType::Hash(const Type* element_type, const unsigned size)
{
    const auto element_hash = element_type->Hash();
    const auto size_hash = std::hash<unsigned>{}(size);
    return CombineHash(CombineHash(element_hash, size_hash), 0x05);
}

Brewer::ArrayType::ArrayType(const unsigned hash, Type* element_type, const unsigned size)
    : Type(hash), m_ElementType(element_type), m_Size(size)
{
}

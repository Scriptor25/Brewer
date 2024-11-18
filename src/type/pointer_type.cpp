#include <Brewer/Type.hpp>

unsigned Brewer::PointerType::Hash(const Type* element_type)
{
    return CombineHash(element_type->Hash(), 0x04);
}

Brewer::PointerType::PointerType(const unsigned hash, Type* element_type)
    : Type(hash), m_ElementType(element_type)
{
}

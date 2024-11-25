#include <Brewer/Type.hpp>

unsigned Brewer::PointerType::Hash(const Type* element_type)
{
    return CombineHash(element_type->GetHash(), 0x04);
}

Brewer::PointerType::PointerType(Context& context, const unsigned hash, Type* element_type)
    : Type(context, hash), m_ElementType(element_type)
{
}

std::ostream& Brewer::PointerType::Print(std::ostream& os) const
{
    return m_ElementType->Print(os << '[') << ']';
}

unsigned Brewer::PointerType::CountBytes() const
{
    return 8;
}

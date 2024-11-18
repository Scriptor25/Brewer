#include <utility>
#include <Brewer/Type.hpp>

unsigned Brewer::StructType::Hash(const std::vector<Type*>& element_types)
{
    unsigned hash = 0x06;
    for (const auto& elem : element_types)
        hash = CombineHash(hash, elem->Hash());
    return hash;
}

Brewer::StructType::StructType(const unsigned hash, std::vector<Type*> element_types)
    : Type(hash), m_ElementTypes(std::move(element_types))
{
}

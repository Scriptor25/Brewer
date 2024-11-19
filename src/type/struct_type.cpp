#include <utility>
#include <Brewer/Type.hpp>

unsigned Brewer::StructType::Hash(const std::vector<Type*>& element_types)
{
    unsigned hash = 0x06;
    for (const auto& elem : element_types)
        hash = CombineHash(hash, elem->Hash());
    return hash;
}

Brewer::StructType::StructType(Context& context, const unsigned hash, std::vector<Type*> element_types)
    : Type(context, hash), m_ElementTypes(std::move(element_types))
{
}

std::ostream& Brewer::StructType::Print(std::ostream& os) const
{
    os << "{ ";
    for (unsigned i = 0; i < m_ElementTypes.size(); ++i)
    {
        if (i > 0) os << ", ";
        m_ElementTypes[i]->Print(os);
    }
    return os << " }";
}

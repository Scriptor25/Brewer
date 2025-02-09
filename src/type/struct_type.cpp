#include <utility>
#include <Brewer/Type.hpp>

unsigned Brewer::StructType::Hash(const std::vector<Type *> &element_types)
{
    unsigned hash = 0x06;
    for (const auto &elem: element_types)
        hash = CombineHash(hash, elem->GetHash());
    return hash;
}

Brewer::StructType::StructType(Context &context, const unsigned hash, std::vector<Type *> &&element_types)
    : Type(context, hash),
      m_ElementTypes(std::move(element_types))
{
}

unsigned Brewer::StructType::GetElementOffset(const unsigned i) const
{
    unsigned offset = 0;
    for (unsigned n = 0; n < i; ++n)
        offset += m_ElementTypes[n]->GetNumBytes();
    return offset;
}

Brewer::Type *Brewer::StructType::GetElementType(const unsigned i) const
{
    return m_ElementTypes[i];
}

unsigned Brewer::StructType::GetNumElements() const
{
    return m_ElementTypes.size();
}

std::ostream &Brewer::StructType::Print(std::ostream &os) const
{
    os << "{ ";
    for (unsigned i = 0; i < m_ElementTypes.size(); ++i)
    {
        if (i > 0)
            os << ", ";
        m_ElementTypes[i]->Print(os);
    }
    return os << " }";
}

unsigned Brewer::StructType::GetNumBytes() const
{
    unsigned bytes = 0;
    for (const auto &ty: m_ElementTypes)
        bytes += ty->GetNumBytes();
    return bytes;
}

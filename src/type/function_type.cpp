#include <utility>
#include <Brewer/Type.hpp>

unsigned Brewer::FunctionType::Hash(const Type* result_type, const std::vector<Type*>& arg_types, const bool vararg)
{
    unsigned hash = CombineHash(result_type->Hash(), 0x07);
    for (const auto& arg : arg_types)
        hash = CombineHash(hash, arg->Hash());
    return CombineHash(hash, std::hash<bool>{}(vararg));
}

Brewer::FunctionType::FunctionType(
    Context& context,
    const unsigned hash,
    Type* result_type,
    std::vector<Type*> arg_types,
    const bool vararg)
    : Type(context, hash), m_ResultType(result_type), m_ArgTypes(std::move(arg_types)), m_VarArg(vararg)
{
}

std::ostream& Brewer::FunctionType::Print(std::ostream& os) const
{
    m_ResultType->Print(os) << '(';
    for (unsigned i = 0; i < m_ArgTypes.size(); ++i)
    {
        if (i > 0) os << ", ";
        m_ArgTypes[i]->Print(os);
    }
    if (m_VarArg)
    {
        if (!m_ArgTypes.empty())
            os << ", ";
        os << "...";
    }
    return os << ')';
}

Brewer::Type* Brewer::FunctionType::ResultType() const
{
    return m_ResultType;
}

Brewer::Type* Brewer::FunctionType::ArgType(const unsigned i) const
{
    return m_ArgTypes[i];
}

bool Brewer::FunctionType::VarArg() const
{
    return m_VarArg;
}

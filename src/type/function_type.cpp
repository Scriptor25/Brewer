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
    const unsigned hash,
    Type* result_type,
    std::vector<Type*> arg_types,
    const bool vararg)
    : Type(hash), m_ResultType(result_type), m_ArgTypes(std::move(arg_types)), m_VarArg(vararg)
{
}

#include <Brewer/Context.hpp>

Brewer::PointerType* Brewer::Context::GetPointerType(Type* element_type)
{
    return GetType<PointerType>(element_type);
}

Brewer::ArrayType* Brewer::Context::GetArrayType(Type* element_type, unsigned size)
{
    return GetType<ArrayType>(element_type, size);
}

Brewer::StructType* Brewer::Context::GetStructType(const std::vector<Type*>& element_types)
{
    return GetType<StructType>(element_types);
}

Brewer::FunctionType* Brewer::Context::GetFunctionType(
    Type* result_type,
    const std::vector<Type*>& arg_types,
    bool vararg)
{
    return GetType<FunctionType>(result_type, arg_types, vararg);
}

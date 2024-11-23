#include <Brewer/Context.hpp>
#include <Brewer/Type.hpp>

Brewer::VoidType* Brewer::Context::GetVoidType()
{
    return GetType<VoidType>();
}

Brewer::BlockType* Brewer::Context::GetBlockType()
{
    return GetType<BlockType>();
}

Brewer::IntType* Brewer::Context::GetIntNType(const unsigned bits)
{
    return GetType<IntType>(bits);
}

Brewer::FloatType* Brewer::Context::GetFloatNType(const unsigned bits)
{
    return GetType<FloatType>(bits);
}

Brewer::PointerType* Brewer::Context::GetPointerType(Type* element_type)
{
    return GetType<PointerType>(element_type);
}

Brewer::ArrayType* Brewer::Context::GetArrayType(Type* element_type, const unsigned size)
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
    const bool vararg)
{
    return GetType<FunctionType>(result_type, arg_types, vararg);
}

Brewer::PointerType* Brewer::Context::GetStringType()
{
    return GetPointerType(GetIntNType(8));
}

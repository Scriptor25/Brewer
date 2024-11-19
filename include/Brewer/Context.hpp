#pragma once

#include <map>
#include <vector>
#include <Brewer/Brewer.hpp>

namespace Brewer
{
    class Context
    {
    public:
        Context() = default;

        VoidType* GetVoidType();
        BlockType* GetBlockType();

        IntType* GetIntNType(unsigned bits);
        FloatType* GetFloatNType(unsigned bits);

        PointerType* GetPointerType(Type* element_type);
        ArrayType* GetArrayType(Type* element_type, unsigned size);
        StructType* GetStructType(const std::vector<Type*>& element_types);
        FunctionType* GetFunctionType(Type* result_type, const std::vector<Type*>& arg_types, bool vararg);

    private:
        template <typename T, typename... Args>
        T* GetType(Args&&... args)
        {
            const unsigned hash = T::Hash(args...);
            auto& type = m_TypeMap[hash];
            if (type) return dynamic_cast<T*>(type);
            return dynamic_cast<T*>(type = new T(*this, hash, args...));
        }

        std::map<unsigned, Type*> m_TypeMap;
    };
}

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

        VoidType *GetVoidType();
        BlockType *GetBlockType();

        IntType *GetIntNType(unsigned bits);
        FloatType *GetFloatNType(unsigned bits);

        PointerType *GetPointerType(Type *element_type);
        ArrayType *GetArrayType(Type *element_type, unsigned size);
        StructType *GetStructType(std::vector<Type *> &&element_types);
        FunctionType *GetFunctionType(Type *result_type, std::vector<Type *> &&arg_types, bool vararg);

        PointerType *GetStringType();

        Type *GlobalType(const std::string &name, Type *type = {});

    private:
        template<typename T, typename... Args>
        T *GetType(Args &&... args)
        {
            const unsigned hash = T::Hash(args...);
            auto &type = m_TypeMap[hash];
            if (type)
                return dynamic_cast<T *>(type);
            return dynamic_cast<T *>(type = new T(*this, hash, std::forward<Args>(args)...));
        }

        std::map<unsigned, Type *> m_TypeMap;
        std::map<std::string, Type *> m_GlobalTypeMap;
    };
}

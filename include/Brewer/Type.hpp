#pragma once

#include <vector>

namespace Brewer
{
    class Type
    {
    public:
        static unsigned CombineHash(unsigned h1, unsigned h2);

        explicit Type(unsigned hash);
        virtual ~Type() = default;

        [[nodiscard]] unsigned Hash() const;

    private:
        unsigned m_Hash;
    };

    class VoidType : public Type
    {
    public:
        static unsigned Hash();

        explicit VoidType(unsigned hash);
    };

    class IntType : public Type
    {
    public:
        static unsigned Hash(unsigned bits);

        IntType(unsigned hash, unsigned bits);

    private:
        unsigned m_Bits;
    };

    class FloatType : public Type
    {
    public:
        static unsigned Hash(unsigned bits);

        FloatType(unsigned hash, unsigned bits);

    private:
        unsigned m_Bits;
    };

    class PointerType : public Type
    {
    public:
        static unsigned Hash(const Type* element_type);

        PointerType(unsigned hash, Type* element_type);

    private:
        Type* m_ElementType;
    };

    class ArrayType : public Type
    {
    public:
        static unsigned Hash(const Type* element_type, unsigned size);

        ArrayType(unsigned hash, Type* element_type, unsigned size);

    private:
        Type* m_ElementType;
        unsigned m_Size;
    };

    class StructType : public Type
    {
    public:
        static unsigned Hash(const std::vector<Type*>& element_types);

        StructType(unsigned hash, std::vector<Type*> element_types);

    private:
        std::vector<Type*> m_ElementTypes;
    };

    class FunctionType : public Type
    {
    public:
        static unsigned Hash(const Type* result_type, const std::vector<Type*>& arg_types, bool vararg);

        FunctionType(unsigned hash, Type* result_type, std::vector<Type*> arg_types, bool vararg);

    private:
        Type* m_ResultType;
        std::vector<Type*> m_ArgTypes;
        bool m_VarArg;
    };
}

#pragma once

#include <format>
#include <sstream>
#include <string>
#include <vector>

namespace Brewer
{
    class Context;

    class Type
    {
    public:
        static unsigned CombineHash(unsigned h1, unsigned h2);

        Type(Context& context, unsigned hash);

        Context& GetContext() const;
        unsigned GetHash() const;
        Type* GetBaseType();

        virtual ~Type() = default;

        virtual unsigned GetBits() const;
        virtual Type* GetElementType() const;
        virtual Type* GetElementType(unsigned i) const;
        virtual unsigned GetNumElements() const;
        virtual Type* GetResultType() const;
        virtual Type* GetArgType(unsigned i) const;
        virtual unsigned GetNumArgs() const;
        virtual bool IsVarArg() const;

        virtual std::ostream& Print(std::ostream& os) const = 0;
        virtual unsigned CountBytes() const = 0;

    private:
        Context& m_Context;
        unsigned m_Hash;
    };

    class VoidType : public Type
    {
    public:
        static unsigned Hash();

        VoidType(Context& context, unsigned hash);

        std::ostream& Print(std::ostream& os) const override;
        unsigned CountBytes() const override;
    };

    class BlockType : public Type
    {
    public:
        static unsigned Hash();

        BlockType(Context& context, unsigned hash);

        std::ostream& Print(std::ostream& os) const override;
        unsigned CountBytes() const override;
    };

    class IntType : public Type
    {
    public:
        static unsigned Hash(unsigned bits);

        IntType(Context& context, unsigned hash, unsigned bits);

        unsigned GetBits() const override;

        std::ostream& Print(std::ostream& os) const override;
        unsigned CountBytes() const override;

    private:
        unsigned m_Bits;
    };

    class FloatType : public Type
    {
    public:
        static unsigned Hash(unsigned bits);

        FloatType(Context& context, unsigned hash, unsigned bits);

        unsigned GetBits() const override;

        std::ostream& Print(std::ostream& os) const override;
        unsigned CountBytes() const override;

    private:
        unsigned m_Bits;
    };

    class PointerType : public Type
    {
    public:
        static unsigned Hash(const Type* element_type);

        PointerType(Context& context, unsigned hash, Type* element_type);

        Type* GetElementType() const override;

        std::ostream& Print(std::ostream& os) const override;
        unsigned CountBytes() const override;

    private:
        Type* m_ElementType;
    };

    class ArrayType : public Type
    {
    public:
        static unsigned Hash(const Type* element_type, unsigned num_elements);

        ArrayType(Context& context, unsigned hash, Type* element_type, unsigned num_elements);

        Type* GetElementType() const override;
        unsigned GetNumElements() const override;

        std::ostream& Print(std::ostream& os) const override;
        unsigned CountBytes() const override;

    private:
        Type* m_ElementType;
        unsigned m_NumElements;
    };

    class StructType : public Type
    {
    public:
        static unsigned Hash(const std::vector<Type*>& element_types);

        StructType(Context& context, unsigned hash, std::vector<Type*>&& element_types);

        unsigned GetElementOffset(unsigned i) const;

        Type* GetElementType(unsigned i) const override;
        unsigned GetNumElements() const override;

        std::ostream& Print(std::ostream& os) const override;
        unsigned CountBytes() const override;

    private:
        std::vector<Type*> m_ElementTypes;
    };

    class FunctionType : public Type
    {
    public:
        static unsigned Hash(const Type* result_type, const std::vector<Type*>& arg_types, bool vararg);

        FunctionType(Context& context, unsigned hash, Type* result_type, std::vector<Type*> arg_types, bool vararg);

        Type* GetResultType() const override;
        Type* GetArgType(unsigned i) const override;
        unsigned GetNumArgs() const override;
        bool IsVarArg() const override;

        std::ostream& Print(std::ostream& os) const override;
        unsigned CountBytes() const override;

    private:
        Type* m_ResultType;
        std::vector<Type*> m_ArgTypes;
        bool m_VarArg;
    };
}

template <typename T>
concept DerivedType = std::is_base_of_v<Brewer::Type, T>;

template <DerivedType T>
struct std::formatter<T*> : formatter<string>
{
    template <typename FormatContext>
    auto format(T* type, FormatContext& ctx) const
    {
        stringstream os;
        type->Print(os);
        return formatter<string>::format(os.str(), ctx);
    }
};

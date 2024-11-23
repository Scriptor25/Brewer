#pragma once

#include <format>
#include <ostream>
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
        virtual ~Type() = default;

        virtual std::ostream& Print(std::ostream& os) const = 0;

        [[nodiscard]] Context& GetContext() const;
        [[nodiscard]] unsigned Hash() const;

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
    };

    class BlockType : public Type
    {
    public:
        static unsigned Hash();

        BlockType(Context& context, unsigned hash);

        std::ostream& Print(std::ostream& os) const override;
    };

    class IntType : public Type
    {
    public:
        static unsigned Hash(unsigned bits);

        IntType(Context& context, unsigned hash, unsigned bits);

        std::ostream& Print(std::ostream& os) const override;

        [[nodiscard]] unsigned Bits() const;

    private:
        unsigned m_Bits;
    };

    class FloatType : public Type
    {
    public:
        static unsigned Hash(unsigned bits);

        FloatType(Context& context, unsigned hash, unsigned bits);

        std::ostream& Print(std::ostream& os) const override;

    private:
        unsigned m_Bits;
    };

    class PointerType : public Type
    {
    public:
        static unsigned Hash(const Type* element_type);

        PointerType(Context& context, unsigned hash, Type* element_type);

        std::ostream& Print(std::ostream& os) const override;

        [[nodiscard]] Type* ElementType() const;

    private:
        Type* m_ElementType;
    };

    class ArrayType : public Type
    {
    public:
        static unsigned Hash(const Type* element_type, unsigned size);

        ArrayType(Context& context, unsigned hash, Type* element_type, unsigned size);

        std::ostream& Print(std::ostream& os) const override;

        template <typename T = Type>
        [[nodiscard]] T* GetElementType() const
        {
            if (!this) return {};
            return dynamic_cast<T*>(m_ElementType);
        }

        [[nodiscard]] unsigned Size() const;

    private:
        Type* m_ElementType;
        unsigned m_Size;
    };

    class StructType : public Type
    {
    public:
        static unsigned Hash(const std::vector<Type*>& element_types);

        StructType(Context& context, unsigned hash, std::vector<Type*> element_types);

        std::ostream& Print(std::ostream& os) const override;

    private:
        std::vector<Type*> m_ElementTypes;
    };

    class FunctionType : public Type
    {
    public:
        static unsigned Hash(const Type* result_type, const std::vector<Type*>& arg_types, bool vararg);

        FunctionType(Context& context, unsigned hash, Type* result_type, std::vector<Type*> arg_types, bool vararg);

        std::ostream& Print(std::ostream& os) const override;

        [[nodiscard]] Type* ResultType() const;
        [[nodiscard]] Type* ArgType(unsigned i) const;
        [[nodiscard]] bool VarArg() const;

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

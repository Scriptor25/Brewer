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
        virtual ~Type() = default;

        virtual std::ostream& Print(std::ostream& os) const = 0;

        [[nodiscard]] Context& GetContext() const;
        [[nodiscard]] unsigned GetHash() const;

        [[nodiscard]] virtual unsigned CountBytes() const = 0;

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
        [[nodiscard]] unsigned CountBytes() const override;
    };

    class BlockType : public Type
    {
    public:
        static unsigned Hash();

        BlockType(Context& context, unsigned hash);

        std::ostream& Print(std::ostream& os) const override;
        [[nodiscard]] unsigned CountBytes() const override;
    };

    class IntType : public Type
    {
    public:
        static unsigned Hash(unsigned bits);

        IntType(Context& context, unsigned hash, unsigned bits);

        std::ostream& Print(std::ostream& os) const override;
        [[nodiscard]] unsigned CountBytes() const override;

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
        [[nodiscard]] unsigned CountBytes() const override;

    private:
        unsigned m_Bits;
    };

    class PointerType : public Type
    {
    public:
        static unsigned Hash(const Type* element_type);

        PointerType(Context& context, unsigned hash, Type* element_type);

        std::ostream& Print(std::ostream& os) const override;
        [[nodiscard]] unsigned CountBytes() const override;

        template <typename T = Type>
        [[nodiscard]] T* GetElementType() const
        {
            return dynamic_cast<T*>(m_ElementType);
        }

    private:
        Type* m_ElementType;
    };

    class ArrayType : public Type
    {
    public:
        static unsigned Hash(const Type* element_type, unsigned num_elements);

        ArrayType(Context& context, unsigned hash, Type* element_type, unsigned num_elements);

        std::ostream& Print(std::ostream& os) const override;
        [[nodiscard]] unsigned CountBytes() const override;

        template <typename T = Type>
        [[nodiscard]] T* GetElementType() const
        {
            return dynamic_cast<T*>(m_ElementType);
        }

        [[nodiscard]] unsigned GetNumElements() const;

    private:
        Type* m_ElementType;
        unsigned m_NumElements;
    };

    class StructType : public Type
    {
    public:
        static unsigned Hash(const std::vector<Type*>& element_types);

        StructType(Context& context, unsigned hash, std::vector<Type*> element_types);

        std::ostream& Print(std::ostream& os) const override;
        [[nodiscard]] unsigned CountBytes() const override;

        template <typename T = Type>
        [[nodiscard]] T* GetElementType(const unsigned i) const
        {
            return dynamic_cast<T*>(m_ElementTypes[i]);
        }

        [[nodiscard]] unsigned GetNumElements() const;

    private:
        std::vector<Type*> m_ElementTypes;
    };

    class FunctionType : public Type
    {
    public:
        static unsigned Hash(const Type* result_type, const std::vector<Type*>& arg_types, bool vararg);

        FunctionType(Context& context, unsigned hash, Type* result_type, std::vector<Type*> arg_types, bool vararg);

        std::ostream& Print(std::ostream& os) const override;
        [[nodiscard]] unsigned CountBytes() const override;

        template <typename T = Type>
        [[nodiscard]] T* GetResultType() const
        {
            return dynamic_cast<T*>(m_ResultType);
        }

        template <typename T = Type>
        [[nodiscard]] T* GetArgType(const unsigned i) const
        {
            return dynamic_cast<T*>(m_ArgTypes[i]);
        }

        [[nodiscard]] unsigned GetNumArgs() const;
        [[nodiscard]] bool IsVarArg() const;

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

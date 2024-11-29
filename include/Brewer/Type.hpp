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

        [[nodiscard]] Context& GetContext() const;
        [[nodiscard]] unsigned GetHash() const;
        Type* GetBaseType();

        virtual ~Type() = default;

        [[nodiscard]] virtual unsigned GetBits() const;
        [[nodiscard]] virtual Type* GetElementType() const;
        [[nodiscard]] virtual Type* GetElementType(unsigned i) const;
        [[nodiscard]] virtual unsigned GetNumElements() const;
        [[nodiscard]] virtual Type* GetResultType() const;
        [[nodiscard]] virtual Type* GetArgType(unsigned i) const;
        [[nodiscard]] virtual unsigned GetNumArgs() const;
        [[nodiscard]] virtual bool IsVarArg() const;

        virtual std::ostream& Print(std::ostream& os) const = 0;
        [[nodiscard]] virtual unsigned GetNumBytes() const = 0;

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
        [[nodiscard]] unsigned GetNumBytes() const override;
    };

    class BlockType : public Type
    {
    public:
        static unsigned Hash();

        BlockType(Context& context, unsigned hash);

        std::ostream& Print(std::ostream& os) const override;
        [[nodiscard]] unsigned GetNumBytes() const override;
    };

    class IntType : public Type
    {
    public:
        static unsigned Hash(unsigned bits);

        IntType(Context& context, unsigned hash, unsigned bits);

        [[nodiscard]] unsigned GetBits() const override;

        std::ostream& Print(std::ostream& os) const override;
        [[nodiscard]] unsigned GetNumBytes() const override;

    private:
        unsigned m_Bits;
    };

    class FloatType : public Type
    {
    public:
        static unsigned Hash(unsigned bits);

        FloatType(Context& context, unsigned hash, unsigned bits);

        [[nodiscard]] unsigned GetBits() const override;

        std::ostream& Print(std::ostream& os) const override;
        [[nodiscard]] unsigned GetNumBytes() const override;

    private:
        unsigned m_Bits;
    };

    class PointerType : public Type
    {
    public:
        static unsigned Hash(const Type* element_type);

        PointerType(Context& context, unsigned hash, Type* element_type);

        [[nodiscard]] Type* GetElementType() const override;

        std::ostream& Print(std::ostream& os) const override;
        [[nodiscard]] unsigned GetNumBytes() const override;

    private:
        Type* m_ElementType;
    };

    class ArrayType : public Type
    {
    public:
        static unsigned Hash(const Type* element_type, unsigned num_elements);

        ArrayType(Context& context, unsigned hash, Type* element_type, unsigned num_elements);

        [[nodiscard]] Type* GetElementType() const override;
        [[nodiscard]] unsigned GetNumElements() const override;

        std::ostream& Print(std::ostream& os) const override;
        [[nodiscard]] unsigned GetNumBytes() const override;

    private:
        Type* m_ElementType;
        unsigned m_NumElements;
    };

    class StructType : public Type
    {
    public:
        static unsigned Hash(const std::vector<Type*>& element_types);

        StructType(Context& context, unsigned hash, std::vector<Type*>&& element_types);

        [[nodiscard]] unsigned GetElementOffset(unsigned i) const;

        [[nodiscard]] Type* GetElementType(unsigned i) const override;
        [[nodiscard]] unsigned GetNumElements() const override;

        std::ostream& Print(std::ostream& os) const override;
        [[nodiscard]] unsigned GetNumBytes() const override;

    private:
        std::vector<Type*> m_ElementTypes;
    };

    class FunctionType : public Type
    {
    public:
        static unsigned Hash(const Type* result_type, const std::vector<Type*>& arg_types, bool vararg);

        FunctionType(Context& context, unsigned hash, Type* result_type, std::vector<Type*> arg_types, bool vararg);

        [[nodiscard]] Type* GetResultType() const override;
        [[nodiscard]] Type* GetArgType(unsigned i) const override;
        [[nodiscard]] unsigned GetNumArgs() const override;
        [[nodiscard]] bool IsVarArg() const override;

        std::ostream& Print(std::ostream& os) const override;
        [[nodiscard]] unsigned GetNumBytes() const override;

    private:
        Type* m_ResultType;
        std::vector<Type*> m_ArgTypes;
        bool m_VarArg;
    };
}

template <typename T>
concept DerivedType = std::is_base_of_v<Brewer::Type, T>;

template <DerivedType T>
struct std::formatter<T*> : std::formatter<std::string>
{
    template <typename FormatContext>
    auto format(T* type, FormatContext& ctx) const
    {
        std::stringstream os;
        type->Print(os);
        return std::formatter<std::string>::format(os.str(), ctx);
    }
};

#pragma once

#include <cstdint>
#include <sstream>
#include <Brewer/Brewer.hpp>

namespace Brewer
{
    class Value
    {
    public:
        Value(Type *type, std::vector<std::string> &&meta);

        [[nodiscard]] unsigned GetIndex() const;
        [[nodiscard]] Type *GetType() const;
        [[nodiscard]] bool GetMeta(const std::string &name) const;

        [[nodiscard]] uint64_t GetNumUses() const;
        void AddUse(Value *user);
        void RemoveUse(const Value *user);
        void ReplaceWith(Value *new_value);
        std::ostream &PrintUseList(std::ostream &os) const;

        virtual ~Value();
        [[nodiscard]] virtual bool NotNull() const;
        [[nodiscard]] virtual bool IsTerminator() const;
        [[nodiscard]] virtual bool RequiresDestination() const;
        [[nodiscard]] virtual uint64_t GetNumAllocaBytes() const;
        virtual void Replace(Value *old_value, Value *new_value);
        virtual std::ostream &PrintIR(std::ostream &os) const;
        virtual std::ostream &PrintOperandIR(std::ostream &os, bool omit_type) const = 0;

    private:
        unsigned m_Index;
        Type *m_Type;

        std::vector<std::string> m_Meta;
        std::vector<Value *> m_UseList;
    };

    class Assignment final : public Value
    {
    public:
        Assignment(Value *dst, Value *src, std::vector<std::string> &&meta);

        [[nodiscard]] Value *GetDst() const;
        [[nodiscard]] Value *GetSrc() const;

        void Replace(Value *old_value, Value *new_value) override;

        std::ostream &PrintIR(std::ostream &os) const override;
        std::ostream &PrintOperandIR(std::ostream &os, bool omit_type) const override;

    private:
        Value *m_Dst;
        Value *m_Src;
    };

    class Instruction final : public Value
    {
    public:
        enum Code
        {
            None,

            Add,
            Sub,

            Call,
            Gep,
            Load,
            Store,
            Alloca,

            Ret,
            Br,
            Br_Lt,
        };

        Instruction(Type *type, Code code, std::vector<Value *> operands, std::vector<std::string> &&meta);

        std::ostream &PrintOperandIR(std::ostream &os, bool omit_type) const override;

        [[nodiscard]] Code GetCode() const;
        [[nodiscard]] Value *GetOperand(unsigned i) const;
        [[nodiscard]] unsigned GetNumOperands() const;
        [[nodiscard]] std::vector<Value *> GetOperandRange(unsigned beg, unsigned end = -1) const;

        [[nodiscard]] bool IsTerminator() const override;
        [[nodiscard]] bool RequiresDestination() const override;
        [[nodiscard]] uint64_t GetNumAllocaBytes() const override;
        void Replace(Value *old_value, Value *new_value) override;

    private:
        Code m_Code;
        std::vector<Value *> m_Operands;
    };

    Instruction::Code ToCode(const std::string &str);
    std::string ToString(Instruction::Code code);
}

template<typename T>
concept DerivedValue = std::is_base_of_v<Brewer::Value, T>;

template<DerivedValue V>
struct std::formatter<V *> : std::formatter<std::string>
{
    template<typename FormatContext>
    auto format(V *value, FormatContext &ctx) const
    {
        std::stringstream ss;
        value->PrintOperandIR(ss, false);
        return std::formatter<std::string>::format(ss.str(), ctx);
    }
};

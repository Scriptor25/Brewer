#pragma once

#include <sstream>
#include <Brewer/Brewer.hpp>

namespace Brewer
{
    class Value
    {
    public:
        explicit Value(Type* type);

        unsigned GetIndex() const;
        Type* GetType() const;

        void AddUse(Value* user);
        void ReplaceWith(Value* new_value);

        virtual ~Value();
        virtual void Replace(Value* old_value, Value* new_value) const;
        virtual std::ostream& PrintIR(std::ostream& os) const;
        virtual std::ostream& PrintOperandIR(std::ostream& os, bool omit_type) const = 0;

    private:
        unsigned m_Index;
        Type* m_Type;

        std::vector<Value*> m_UseList;
    };

    class Assignment : public Value
    {
    public:
        Assignment(Value* dst, Value* src);

        std::ostream& PrintIR(std::ostream& os) const override;
        std::ostream& PrintOperandIR(std::ostream& os, bool omit_type) const override;

    private:
        Value* m_Dst;
        Value* m_Src;
    };

    class Instruction : public Value
    {
    public:
        enum Code
        {
            None,

            Add,
            Sub,

            Call,
            Gep,

            Ret,
            Br,
            Br_Lt,
        };

        Instruction(Type* type, Code code, std::vector<Value*> operands);

        std::ostream& PrintOperandIR(std::ostream& os, bool omit_type) const override;

        Code GetCode() const;
        Value* GetOperand(unsigned i) const;
        unsigned GetNumOperands() const;
        std::vector<Value*> GetOperandRange(unsigned beg, unsigned end = -1) const;

    private:
        Code m_Code;
        std::vector<Value*> m_Operands;
    };

    Instruction::Code ToCode(const std::string& str);
    std::string ToString(Instruction::Code code);
}

namespace std
{
    template <typename T>
    concept DerivedValue = std::is_base_of_v<Brewer::Value, T>;

    template <DerivedValue V>
    struct formatter<V*> : formatter<string>
    {
        template <typename FormatContext>
        auto format(V* value, FormatContext& ctx) const
        {
            std::stringstream ss;
            value->PrintIR(ss);
            return formatter<string>::format(ss.str(), ctx);
        }
    };
}

#pragma once

#include <map>
#include <Brewer/Printer/Printer.hpp>

namespace Brewer
{
    class X86Printer : public Printer
    {
    public:
        enum Register
        {
            NLL,

            RAX, RBX, RCX, RDX, RSI, RDI, RSP, RBP,
            R8, R9, R10, R11, R12, R13, R14, R15,
        };

        struct Storage
        {
            static bool Equal(const Storage* lhs, const Storage* rhs);

            virtual ~Storage() = default;
            virtual void Print(X86Printer& printer, unsigned bytes) const = 0;
            virtual bool Equals(const Storage* other) const = 0;
        };

        struct RegisterStorage : Storage
        {
            explicit RegisterStorage(Register reg);
            void Print(X86Printer& printer, unsigned bytes) const override;
            bool Equals(const Storage* other) const override;

            Register Reg;
        };

        struct MemoryStorage : Storage
        {
            MemoryStorage(int segment, int displacement, Register base, Register index, int scale);
            void Print(X86Printer& printer, unsigned bytes) const override;
            bool Equals(const Storage* other) const override;

            int Segment;
            int Displacement;
            Register Base;
            Register Index;
            int Scale;
        };

        struct ImmediateStorage : Storage
        {
            explicit ImmediateStorage(int value);
            void Print(X86Printer& printer, unsigned bytes) const override;
            bool Equals(const Storage* other) const override;

            int Value;
        };

        explicit X86Printer(std::ostream& stream);

        void Print(Module* module) override;
        void Print(Value* value) override;

    private:
        void op(const std::string& name, const std::vector<const Storage*>& operands, unsigned bytes);
        void mov(const Storage* src, const Storage* dst, unsigned bytes);
        void push(const Storage* src, unsigned bytes);
        void pop(const Storage* dst, unsigned bytes);
        void add(const Storage* src, const Storage* dst, unsigned bytes);
        void sub(const Storage* src, const Storage* dst, unsigned bytes);
        void imul(const Storage* src, const Storage* dst, unsigned bytes);
        void cmp(const Storage* l, const Storage* r, unsigned bytes);
        void lea(const Storage* src, const Storage* dst, unsigned bytes);

        void op(const std::string& name, Value* value, const Storage* dst = {});
        void mov(Value* value, const Storage* dst);
        void push(Value* value);
        void pop(Value* value);
        void add(Value* value, const Storage* dst);
        void sub(Value* value, const Storage* dst);
        void imul(Value* value, const Storage* dst);
        void cmp(Value* value, const Storage* dst);
        void lea(Value* value, const Storage* dst);

        void ret() const;
        void call(Value* value, const Storage* dst);
        void jmp(Value* value);
        void jl(Value* value);

        void BeginFrame();
        int GetOffset(Value* value);

        void PrintType(Type* type);
        void PrintType(IntType* type);
        void PrintType(FloatType* type);
        void PrintType(PointerType* type);
        void PrintType(ArrayType* type);

        void PrintGlobal(Value* value);
        void PrintGlobal(NamedValue* value);
        void PrintGlobal(GlobalValue* value);
        void PrintGlobal(GlobalVariable* value);
        void PrintGlobal(GlobalFunction* value);
        void PrintGlobal(FunctionBlock* value);

        void PrintGlobalOperand(Value* value);
        void PrintGlobalOperand(Constant* value);
        void PrintGlobalOperand(ConstantInt* value);
        void PrintGlobalOperand(ConstantArray* value);

        void Print(Value* value, const Storage* dst);
        void Print(Assignment* value);
        void Print(Constant* value, const Storage* dst);
        void Print(ConstantInt* value, const Storage* dst);
        void Print(Instruction* value, const Storage* dst);
        void Print(NamedValue* value, const Storage* dst);
        void Print(FunctionBlock* value, const Storage* dst);
        void Print(GlobalValue* value, const Storage* dst);

        void PrintOperand(Value* value);
        void PrintOperand(Assignment* value);
        void PrintOperand(Constant* value);
        void PrintOperand(ConstantInt* value);
        void PrintOperand(ConstantFloat* value);
        void PrintOperand(NamedValue* value);
        void PrintOperand(FunctionBlock* value);
        void PrintOperand(GlobalValue* value);

        void PrintCallee(Value* value);
        void PrintCallee(NamedValue* value);
        void PrintCallee(FunctionBlock* value);
        void PrintCallee(GlobalValue* value);

        static const std::vector<Register> CALL_REGISTERS;

        std::map<Value*, int> m_Offsets;
        int m_Top = 0;
    };
}

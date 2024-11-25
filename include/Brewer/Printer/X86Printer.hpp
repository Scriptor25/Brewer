#pragma once

#include <Brewer/Printer/Printer.hpp>

namespace Brewer
{
    class X86Printer : public Printer
    {
    public:
        enum Register
        {
            RAX, RBX, RCX, RDX, RSI, RDI, RSP, RBP,
            R8, R9, R10, R11, R12, R13, R14, R15,
        };

        explicit X86Printer(std::ostream& stream);

        void Print(Module* module) override;
        void Print(Value* value) override;

    private:
        void mov(Value* src, Value* dst);
        void mov(Value* src, Register dst);
        void mov(Register src, Register dst, int dst_off, unsigned bytes);
        void push(Value* src);
        void pop(Value* dst);
        void call(Value* callee);
        void lea(Value* src, Register dst);
        void imul(Value* src, Register dst);
        void cmp(Value* l_src, Value* r_src);
        void add(Value* src, Register dst);
        void sub(Value* src, Register dst);

        void BeginFrame();
        unsigned GetOffset(Value* value);
        std::map<Value*, unsigned> m_Offsets;
        unsigned m_Top = 0u;

        void Print(Type* type);
        void Print(IntType* type);
        void Print(FloatType* type);
        void Print(PointerType* type);
        void Print(ArrayType* type);

        void Print(Constant* value);
        void Print(ConstantArray* value);
        void Print(GlobalValue* value);
        void Print(GlobalVariable* value);
        void Print(GlobalFunction* value);
        void Print(FunctionBlock* value);
        void Print(Instruction* value);

        void PrintGlobalOperand(Value* value);
        void PrintGlobalOperand(Constant* value);
        void PrintGlobalOperand(ConstantInt* value);
        void PrintGlobalOperand(ConstantArray* value);

        void PrintOperand(Value* value);
        void PrintOperand(Constant* value);
        void PrintOperand(ConstantInt* value);
        void PrintOperand(ConstantFloat* value);
        void PrintOperand(GlobalValue* value);
        void PrintOperand(GlobalVariable* value);

        void PrintCallee(Value* value);
        void PrintCallee(GlobalValue* value);
        void PrintCallee(GlobalFunction* value);
        void PrintCallee(FunctionBlock* value);
    };
}

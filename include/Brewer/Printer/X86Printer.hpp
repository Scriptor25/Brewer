#pragma once

#include <map>
#include <Brewer/Printer/Printer.hpp>

namespace Brewer
{
    class X86Printer : public Printer
    {
    public:
        explicit X86Printer(std::ostream& stream);

        void Print(Module* module) override;
        void Print(Value* value) override;

    private:
        void Print(Type* type);
        void Print(IntType* type);
        void Print(FloatType* type);
        void Print(PointerType* type);
        void Print(ArrayType* type);

        void Print(Constant* value);
        void Print(ConstantInt* value);
        void Print(ConstantFloat* value);
        void Print(ConstantArray* value);
        void Print(NamedValue* value);
        void Print(GlobalValue* value);
        void Print(GlobalVariable* value);
        void Print(Function* value);
        void Print(Block* value);
        void Print(Instruction* value);

        void PrintOperand(Value* value);
        void PrintOperand(Constant* value);
        void PrintOperand(ConstantInt* value);
        void PrintOperand(ConstantFloat* value);
        void PrintOperand(ConstantArray* value);
        void PrintOperand(NamedValue* value);
        void PrintOperand(GlobalValue* value);
        void PrintOperand(Function* value);

        std::map<Value*, bool> m_Printed;
    };
}

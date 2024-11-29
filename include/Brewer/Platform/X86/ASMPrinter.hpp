#pragma once

#include <cstdint>
#include <map>
#include <vector>
#include <Brewer/Brewer.hpp>
#include <Brewer/Printer.hpp>

namespace Brewer::Platform::X86
{
    class ASMPrinter : public ASMPrinterBase
    {
        enum Register
        {
            ZERO,

            RA, RB, RC, RD, RSI, RDI, RSP, RBP,
            R8, R9, R10, R11, R12, R13, R14, R15,
        };

        struct Storage
        {
            static bool Equal(const Storage& lhs, const Storage& rhs);

            Storage();
            explicit Storage(int64_t value);
            explicit Storage(uint64_t value);
            explicit Storage(Register reg);
            Storage(int64_t segment, int64_t displacement, Register base, Register index, int64_t scale);

            void Print(const ASMPrinter& printer, unsigned bytes) const;
            explicit operator bool() const;

            bool Valid = false;
            bool Immediate = false;
            bool Direct = false;
            int64_t Segment = 0;
            int64_t Displacement = 0;
            Register Base = ZERO;
            Register Index = ZERO;
            int64_t Scale = 0;
        };

        static std::string to_string(Register reg, unsigned bytes);

        static const std::vector<Register> CALL_REGISTERS;

    public:
        explicit ASMPrinter(std::ostream& stream);

        void Print(Module& module) override;
        void Print(Value* value) override;

    private:
        bool CanOmitMov(const Storage& src, const Storage& dst);
        void SetLast(const Storage& src, const Storage& dst);
        void ClearLast();

        void asm_op(const std::string& name, const std::vector<Storage>& operands, unsigned bytes);
        void asm_mov(const Storage& src, const Storage& dst, unsigned bytes);
        void asm_push(const Storage& src, unsigned bytes);
        void asm_pop(const Storage& dst, unsigned bytes);
        void asm_add(const Storage& src, const Storage& dst, unsigned bytes);
        void asm_sub(const Storage& src, const Storage& dst, unsigned bytes);
        void asm_imul(const Storage& src, const Storage& dst, unsigned bytes);
        void asm_cmp(const Storage& l, const Storage& r, unsigned bytes);
        void asm_lea(const Storage& src, const Storage& dst, unsigned bytes);

        void asm_op(const std::string& name, Value* value, const Storage& dst = {});
        void asm_mov(Value* value, const Storage& dst);
        void asm_push(Value* value);
        void asm_pop(Value* value);
        void asm_add(Value* value, const Storage& dst);
        void asm_sub(Value* value, const Storage& dst);
        void asm_imul(Value* value, const Storage& dst);
        void asm_cmp(Value* value, const Storage& dst);
        void asm_lea(Value* value, const Storage& dst);

        void asm_ret() const;
        void asm_call(Value* value, const Storage& dst);
        void asm_jmp(Value* value);
        void asm_jl(Value* value);

        void BeginFrame();
        int64_t GetOffset(Value* value);

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

        void Print(Value* value, const Storage& dst);
        void Print(Assignment* value);
        void Print(Constant* value, const Storage& dst);
        void Print(ConstantInt* value, const Storage& dst);
        void Print(Instruction* value, const Storage& dst);
        void Print(NamedValue* value, const Storage& dst);
        void Print(FunctionBlock* value, const Storage& dst);
        void Print(GlobalValue* value, const Storage& dst);

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

        std::map<Value*, int64_t> m_Offsets;
        int64_t m_Offset = 0;
        int64_t m_Top = 0;

        Storage m_LastSrc;
        Storage m_LastDst;
    };
}

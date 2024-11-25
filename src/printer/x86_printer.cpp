#include <map>
#include <Brewer/Module.hpp>
#include <Brewer/Type.hpp>
#include <Brewer/Printer/X86Printer.hpp>
#include <Brewer/Value/Constant.hpp>
#include <Brewer/Value/GlobalValue.hpp>
#include <Brewer/Value/Value.hpp>

#include "Brewer/Value/NamedValue.hpp"

static std::string to_string(const Brewer::X86Printer::Register reg, const unsigned bytes)
{
    static std::map<Brewer::X86Printer::Register, std::string> gp_regs
    {
        {Brewer::X86Printer::RAX, "a"},
        {Brewer::X86Printer::RBX, "b"},
        {Brewer::X86Printer::RCX, "c"},
        {Brewer::X86Printer::RDX, "d"},
    };
    static std::map<Brewer::X86Printer::Register, std::string> sp_regs
    {
        {Brewer::X86Printer::RSI, "si"},
        {Brewer::X86Printer::RDI, "di"},
        {Brewer::X86Printer::RSP, "sp"},
        {Brewer::X86Printer::RBP, "bp"},
    };
    static std::map<Brewer::X86Printer::Register, std::string> ex_regs
    {
        {Brewer::X86Printer::R8, "r8"},
        {Brewer::X86Printer::R9, "r9"},
        {Brewer::X86Printer::R10, "r10"},
        {Brewer::X86Printer::R11, "r11"},
        {Brewer::X86Printer::R12, "r12"},
        {Brewer::X86Printer::R13, "r13"},
        {Brewer::X86Printer::R14, "r14"},
        {Brewer::X86Printer::R15, "r15"},
    };

    if (gp_regs.contains(reg))
    {
        std::string res = gp_regs[reg];
        switch (bytes)
        {
        case 0:
            res = res + 'h';
            break;
        case 1:
            res = res + 'l';
            break;
        case 2:
            res = res + 'x';
            break;
        case 4:
            res = 'e' + res + 'x';
            break;
        case 8:
            res = 'r' + res + 'x';
            break;
        default: break;
        }
        return res;
    }
    if (sp_regs.contains(reg))
    {
        std::string res = sp_regs[reg];
        switch (bytes)
        {
        case 0:
            res = res + 'h';
            break;
        case 1:
            res = res + 'l';
            break;
        case 2: break;
        case 4:
            res = 'e' + res;
            break;
        case 8:
            res = 'r' + res;
            break;
        default: break;
        }
        return res;
    }

    std::string res = ex_regs[reg];
    switch (bytes)
    {
    case 0:
    case 1:
        res = res + 'b';
        break;
    case 2:
        res = res + 'w';
        break;
    case 4:
        res = res + 'd';
        break;
    default: break;
    }
    return res;
}

static std::vector call_registers
{
    Brewer::X86Printer::RCX,
    Brewer::X86Printer::RDX,
    Brewer::X86Printer::R8,
    Brewer::X86Printer::R9,
};

Brewer::X86Printer::X86Printer(std::ostream& stream)
    : Printer(stream)
{
}

void Brewer::X86Printer::Print(Module* module)
{
    S() << ".file \"" << module->GetFilename() << '"' << std::endl;
    module->ForEach([&](GlobalValue* value)
    {
        Print(value);
    });
}

void Brewer::X86Printer::Print(Value* value)
{
    if (const auto ptr = dynamic_cast<FunctionBlock*>(value))
        return Print(ptr);
    if (const auto ptr = dynamic_cast<Constant*>(value))
        return Print(ptr);
    if (const auto ptr = dynamic_cast<GlobalValue*>(value))
        return Print(ptr);
    if (const auto ptr = dynamic_cast<Instruction*>(value))
        return Print(ptr);

    Error("X86Printer::Print(Value*) not implemented: {}", value);
}

void Brewer::X86Printer::mov(Value* src, Value* dst)
{
    static std::map<unsigned, std::string> mov
    {
        {1, "movb"},
        {2, "movw"},
        {4, "movl"},
        {8, "movq"},
    };
    S() << mov[src->GetType()->CountBytes()] << ' ';
    PrintOperand(src);
    S() << ", ";
    PrintOperand(dst);
    S() << std::endl;
}

void Brewer::X86Printer::mov(Value* src, const Register dst)
{
    static std::map<unsigned, std::string> mov
    {
        {1, "movb"},
        {2, "movw"},
        {4, "movl"},
        {8, "movq"},
    };
    const auto bytes = src->GetType()->CountBytes();
    S() << mov[bytes] << ' ';
    PrintOperand(src);
    S() << ", %" << to_string(dst, bytes) << std::endl;
}

void Brewer::X86Printer::mov(
    const Register src,
    const Register dst,
    const int dst_off,
    const unsigned bytes)
{
    static std::map<unsigned, std::string> mov
    {
        {1, "movb"},
        {2, "movw"},
        {4, "movl"},
        {8, "movq"},
    };
    S() << mov[bytes] << " %" << to_string(src, bytes) << ", "
        << dst_off << "(%" << to_string(dst, 8) << ')' << std::endl;
}

void Brewer::X86Printer::push(Value* src)
{
    static std::map<unsigned, std::string> push
    {
        {1, "pushb"},
        {2, "pushw"},
        {4, "pushl"},
        {8, "pushq"},
    };
    S() << push[src->GetType()->CountBytes()] << ' ';
    PrintOperand(src);
    S() << std::endl;
}

void Brewer::X86Printer::pop(Value* dst)
{
    static std::map<unsigned, std::string> push
    {
        {1, "popb"},
        {2, "popw"},
        {4, "popl"},
        {8, "popq"},
    };
    S() << push[dst->GetType()->CountBytes()] << ' ';
    PrintOperand(dst);
    S() << std::endl;
}

void Brewer::X86Printer::call(Value* callee)
{
    S() << "call ";
    PrintCallee(callee);
    S() << std::endl;
}

void Brewer::X86Printer::lea(Value* src, const Register dst)
{
    S() << "leaq ";
    PrintOperand(src);
    S() << ", %" << to_string(dst, 8) << std::endl;
}

void Brewer::X86Printer::imul(Value* src, const Register dst)
{
    static std::map<unsigned, std::string> imul
    {
        {1, "imulb"},
        {2, "imulw"},
        {4, "imull"},
        {8, "imulq"},
    };
    const auto bytes = src->GetType()->CountBytes();
    S() << imul[bytes] << ' ';
    PrintOperand(src);
    S() << ", %" << to_string(dst, bytes) << std::endl;
}

void Brewer::X86Printer::cmp(Value* l_src, Value* r_src)
{
    static std::map<unsigned, std::string> cmp
    {
        {1, "cmpb"},
        {2, "cmpw"},
        {4, "cmpl"},
        {8, "cmpq"},
    };
    const auto bytes = l_src->GetType()->CountBytes();
    mov(l_src, RAX);
    mov(r_src, RBX);
    S() << cmp[bytes] << " %" << to_string(RAX, bytes) << ", %" << to_string(RBX, bytes) << std::endl;
}

void Brewer::X86Printer::add(Value* src, const Register dst)
{
    static std::map<unsigned, std::string> add
    {
        {1, "addb"},
        {2, "addw"},
        {4, "addl"},
        {8, "addq"},
    };
    const auto bytes = src->GetType()->CountBytes();
    S() << add[bytes] << ' ';
    PrintOperand(src);
    S() << ", %" << to_string(dst, bytes) << std::endl;
}

void Brewer::X86Printer::sub(Value* src, Register dst)
{
    static std::map<unsigned, std::string> sub
    {
        {1, "subb"},
        {2, "subw"},
        {4, "subl"},
        {8, "subq"},
    };
    const auto bytes = src->GetType()->CountBytes();
    S() << sub[bytes] << ' ';
    PrintOperand(src);
    S() << ", %" << to_string(dst, bytes) << std::endl;
}

void Brewer::X86Printer::BeginFrame()
{
    m_Offsets.clear();
    m_Top = 0;
}

unsigned Brewer::X86Printer::GetOffset(Value* value)
{
    if (m_Offsets.contains(value))
        return m_Offsets[value];
    const auto result = m_Offsets[value] = m_Top;
    m_Top += value->GetType()->CountBytes();
    return result;
}

void Brewer::X86Printer::Print(Type* type)
{
    if (const auto ptr = dynamic_cast<IntType*>(type))
        return Print(ptr);
    if (const auto ptr = dynamic_cast<FloatType*>(type))
        return Print(ptr);
    if (const auto ptr = dynamic_cast<PointerType*>(type))
        return Print(ptr);
    if (const auto ptr = dynamic_cast<ArrayType*>(type))
        return Print(ptr);

    Error("X86Printer::Print(Type*) not implemented: {}", type);
}

void Brewer::X86Printer::Print(IntType* type)
{
    switch (type->GetBits())
    {
    case 8:
        S() << ".byte";
        return;
    case 16:
        S() << ".word";
        return;
    case 32:
        S() << ".long";
        return;
    case 64:
        S() << ".quad";
        return;

    default:
        break;
    }

    Error("X86Printer::Print(IntType*) not implemented: {}", type);
}

void Brewer::X86Printer::Print(FloatType* type)
{
    switch (type->GetBits())
    {
    case 32:
        S() << ".float";
        return;
    case 64:
        S() << ".double";
        return;

    default:
        break;
    }

    Error("X86Printer::Print(FloatType*) not implemented: {}", type);
}

void Brewer::X86Printer::Print(PointerType*)
{
    S() << ".quad";
}

void Brewer::X86Printer::Print(ArrayType*)
{
    S() << ".quad";
}

void Brewer::X86Printer::Print(Constant* value)
{
    if (const auto ptr = dynamic_cast<ConstantArray*>(value))
        return Print(ptr);

    Error("X86Printer::Print(Constant*) not implemented: {}", value);
}

void Brewer::X86Printer::Print(ConstantArray* value)
{
    S() << ".section .rodata" << std::endl;
    S() << ".LC" << value->GetIndex() << ": ";
    Print(value->GetType()->GetElementType());
    S() << ' ';
    for (unsigned i = 0; i < value->GetNumVals(); ++i)
    {
        if (i > 0) S() << ", ";
        PrintGlobalOperand(value->GetVal(i));
    }
    S() << std::endl;
}

void Brewer::X86Printer::Print(GlobalValue* value)
{
    if (const auto ptr = dynamic_cast<GlobalVariable*>(value))
        return Print(ptr);
    if (const auto ptr = dynamic_cast<GlobalFunction*>(value))
        return Print(ptr);

    Error("X86Printer::Print(GlobalValue*) not implemented: {}", value);
}

void Brewer::X86Printer::Print(GlobalVariable* value)
{
    S() << ".section .data" << std::endl;
    switch (value->GetLinkage())
    {
    case GlobalValue::Linkage_Global:
        S() << ".global " << value->GetName() << std::endl;
        break;
    case GlobalValue::Linkage_Weak:
        S() << ".weak " << value->GetName() << std::endl;
        break;
    default: break;
    }

    if (!value->GetInit())
    {
        S() << ".extern " << value->GetName() << std::endl;
        return;
    }

    S() << value->GetName() << ": ";
    Print(value->GetInit()->GetType());
    S() << ' ';
    PrintGlobalOperand(value->GetInit());
    S() << std::endl;

    Print(value->GetInit());
}

void Brewer::X86Printer::Print(GlobalFunction* value)
{
    S() << ".section .text" << std::endl;
    switch (value->GetLinkage())
    {
    case GlobalValue::Linkage_Global:
        S() << ".global " << value->GetName() << std::endl;
        break;
    case GlobalValue::Linkage_Weak:
        S() << ".weak " << value->GetName() << std::endl;
        break;
    default: break;
    }

    if (value->IsEmpty())
    {
        S() << ".extern " << value->GetName() << std::endl;
        return;
    }

    S() << value->GetName() << ':' << std::endl;

    S() << "pushq %rbp" << std::endl;
    S() << "movq %rsp, %rbp" << std::endl;
    S() << "subq $" << value->GetByteAlloc() << ", %rsp" << std::endl;

    for (unsigned i = 0; i < value->GetNumArgs(); ++i)
    {
        const auto arg = value->GetArg(i);
        const auto offset = GetOffset(arg);
        if (i < call_registers.size())
            mov(call_registers[i], RBP, -offset, arg->GetType()->CountBytes());
    }

    for (unsigned i = 0; i < value->GetNumBlocks(); ++i)
        Print(value->GetBlock(i));
}

void Brewer::X86Printer::Print(FunctionBlock* value)
{
    S() << ".L" << value->GetIndex() << ':' << std::endl;
    for (unsigned i = 0; i < value->GetNumValues(); ++i)
        Print(value->GetValue(i));
}

void Brewer::X86Printer::Print(Instruction* value)
{
    switch (value->GetCode())
    {
    case Instruction::Add:
        {
            const auto l_src = value->GetOperand(0);
            const auto r_src = value->GetOperand(1);

            mov(l_src, RAX);
            add(r_src, RAX);

            if (value->GetType())
                mov(RAX, RBP, -GetOffset(value), value->GetType()->CountBytes());
        }
        return;
    case Instruction::Sub:
        {
            const auto l_src = value->GetOperand(0);
            const auto r_src = value->GetOperand(1);

            mov(l_src, RAX);
            sub(r_src, RAX);

            if (value->GetType())
                mov(RAX, RBP, -GetOffset(value), value->GetType()->CountBytes());
        }
        return;
    case Instruction::Call:
        {
            const auto callee = value->GetOperand(0);
            const auto args = value->GetOperandRange(1);

            for (unsigned i = 0; i < args.size() && i < call_registers.size(); ++i)
                mov(args[i], call_registers[i]);
            for (unsigned i = args.size(); i > call_registers.size(); --i)
                push(args[i - 1]);

            call(callee);

            if (value->GetType())
                mov(RAX, RBP, -GetOffset(value), value->GetType()->CountBytes());
        }
        return;
    case Instruction::Gep:
        {
            const auto base = value->GetOperand(0);
            auto type = base->GetType();
            lea(base, RAX);

            const auto indices = value->GetOperandRange(1);
            for (unsigned i = 0; i < indices.size(); ++i)
            {
                const auto index = indices[i];

                if (const auto pt = dynamic_cast<PointerType*>(type))
                    type = pt->GetElementType();
                else if (const auto at = dynamic_cast<ArrayType*>(type))
                    type = at->GetElementType();

                const auto bytes = type->CountBytes();
                mov(index, RBX);
                S() << "imulq $" << bytes << ", %rbx" << std::endl;
                S() << "addq %rbx, %rax" << std::endl;

                if (i < indices.size() - 1)
                    S() << "movq (%rax), %rax" << std::endl;
            }

            if (value->GetType())
                mov(RAX, RBP, -GetOffset(value), value->GetType()->CountBytes());
        }
        return;
    case Instruction::Ret:
        {
            const auto result = value->GetOperand(0);

            mov(result, RAX);
            S() << "movq %rbp, %rsp" << std::endl;
            S() << "popq %rbp" << std::endl;
            S() << "ret" << std::endl;
        }
        return;
    case Instruction::Br:
        {
            const auto dst = value->GetOperand(0);
            S() << "jmp ";
            PrintCallee(dst);
            S() << std::endl;
        }
        return;
    case Instruction::Br_Lt:
        {
            const auto l_src = value->GetOperand(0);
            const auto r_src = value->GetOperand(1);
            const auto then_dst = value->GetOperand(2);
            const auto else_dst = value->GetOperand(3);
            cmp(l_src, r_src);
            S() << "jl ";
            PrintCallee(then_dst);
            S() << std::endl;
            S() << "jmp ";
            PrintCallee(else_dst);
            S() << std::endl;
        }
        return;

    default: break;
    }

    Error("X86Printer::Print(Instruction*) not implemented: {}", value);
}

void Brewer::X86Printer::PrintGlobalOperand(Value* value)
{
    if (const auto ptr = dynamic_cast<Constant*>(value))
        return PrintGlobalOperand(ptr);

    Error("X86Printer::PrintGlobalOperand(Value*) not implemented: {}", value);
}

void Brewer::X86Printer::PrintGlobalOperand(Constant* value)
{
    if (const auto ptr = dynamic_cast<ConstantInt*>(value))
        return PrintGlobalOperand(ptr);
    if (const auto ptr = dynamic_cast<ConstantArray*>(value))
        return PrintGlobalOperand(ptr);

    Error("X86Printer::PrintGlobalOperand(Constant*) not implemented: {}", value);
}

void Brewer::X86Printer::PrintGlobalOperand(ConstantInt* value)
{
    S() << value->GetVal();
}

void Brewer::X86Printer::PrintGlobalOperand(ConstantArray* value)
{
    S() << ".LC" << value->GetIndex();
}

void Brewer::X86Printer::PrintOperand(Value* value)
{
    if (const auto ptr = dynamic_cast<Constant*>(value))
        return PrintOperand(ptr);
    if (const auto ptr = dynamic_cast<GlobalValue*>(value))
        return PrintOperand(ptr);

    S() << '-' << GetOffset(value) << "(%rbp)";
}

void Brewer::X86Printer::PrintOperand(Constant* value)
{
    if (const auto ptr = dynamic_cast<ConstantInt*>(value))
        return PrintOperand(ptr);
    if (const auto ptr = dynamic_cast<ConstantFloat*>(value))
        return PrintOperand(ptr);

    Error("X86Printer::PrintOperand(Constant*) not implemented: {}", value);
}

void Brewer::X86Printer::PrintOperand(ConstantInt* value)
{
    S() << '$' << value->GetVal();
}

void Brewer::X86Printer::PrintOperand(ConstantFloat* value)
{
    Error("X86Printer::PrintOperand(ConstantFloat*) not implemented: {}", value);
}

void Brewer::X86Printer::PrintOperand(GlobalValue* value)
{
    if (const auto ptr = dynamic_cast<GlobalVariable*>(value))
        return PrintOperand(ptr);

    Error("X86Printer::PrintOperand(GlobalValue*) not implemented: {}", value);
}

void Brewer::X86Printer::PrintOperand(GlobalVariable* value)
{
    S() << value->GetName() << "(%rip)";
}

void Brewer::X86Printer::PrintCallee(Value* value)
{
    if (const auto ptr = dynamic_cast<GlobalValue*>(value))
        return PrintCallee(ptr);
    if (const auto ptr = dynamic_cast<FunctionBlock*>(value))
        return PrintCallee(ptr);

    Error("X86Printer::PrintCallee(Value*) not implemented: {}", value);
}

void Brewer::X86Printer::PrintCallee(GlobalValue* value)
{
    if (const auto ptr = dynamic_cast<GlobalFunction*>(value))
        return PrintCallee(ptr);

    Error("X86Printer::PrintCallee(GlobalValue*) not implemented: {}", value);
}

void Brewer::X86Printer::PrintCallee(GlobalFunction* value)
{
    S() << value->GetName();
}

void Brewer::X86Printer::PrintCallee(FunctionBlock* value)
{
    S() << ".L" << value->GetIndex();
}

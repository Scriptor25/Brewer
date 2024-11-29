#include <Brewer/Module.hpp>
#include <Brewer/Type.hpp>
#include <Brewer/Printer/X86Printer.hpp>
#include <Brewer/Value/Constant.hpp>
#include <Brewer/Value/GlobalValue.hpp>
#include <Brewer/Value/NamedValue.hpp>
#include <Brewer/Value/Value.hpp>

void Brewer::X86Printer::Print(Module* module)
{
    S() << ".file \"" << module->GetFilename() << '"' << std::endl;
    S() << ".ident \"Brewer\"" << std::endl;
    module->ForEach([&](GlobalValue* value)
    {
        S() << std::endl;
        PrintGlobal(value);
    });
}

void Brewer::X86Printer::Print(Value* value)
{
    return Print(value, {});
}

void Brewer::X86Printer::Print(Value* value, const Storage& dst)
{
    if (const auto ptr = dynamic_cast<Assignment*>(value))
        return Print(ptr);
    if (const auto ptr = dynamic_cast<Constant*>(value))
        return Print(ptr, dst);
    if (const auto ptr = dynamic_cast<Instruction*>(value))
        return Print(ptr, dst);
    if (const auto ptr = dynamic_cast<NamedValue*>(value))
        return Print(ptr, dst);

    Error("X86Printer::Print(Value*) not implemented: {}", value);
}

void Brewer::X86Printer::Print(Assignment* value)
{
    const auto src = value->GetSrc();
    const auto dst = value->GetDst();
    const auto bytes = src->GetType()->CountBytes();
    const auto displacement = GetOffset(dst);

    const Storage rax(RAX);
    const Storage storage(0, displacement, RBP, NLL, 0);
    Print(src, rax);
    mov(rax, storage, bytes);
}

void Brewer::X86Printer::Print(Constant* value, const Storage& dst)
{
    if (const auto ptr = dynamic_cast<ConstantInt*>(value))
        return Print(ptr, dst);

    Error("X86Printer::Print(Constant*) not implemented: {}", value);
}

void Brewer::X86Printer::Print(ConstantInt* value, const Storage& dst)
{
    const auto bytes = value->GetType()->CountBytes();
    const Storage imm(value->GetVal());
    mov(imm, dst, bytes);
}

void Brewer::X86Printer::Print(Instruction* value, const Storage& dst)
{
    switch (value->GetCode())
    {
    case Instruction::Add:
        {
            const auto l_src = value->GetOperand(0);
            const auto r_src = value->GetOperand(1);

            mov(l_src, dst);
            add(r_src, dst);
        }
        return;
    case Instruction::Sub:
        {
            const auto l_src = value->GetOperand(0);
            const auto r_src = value->GetOperand(1);

            mov(l_src, dst);
            sub(r_src, dst);
        }
        return;
    case Instruction::Call:
        {
            // Microsoft x64

            const auto callee = value->GetOperand(0);
            const auto args = value->GetOperandRange(1);

            auto displacement = m_Top;
            for (unsigned i = args.size(); i > 0; --i)
            {
                const auto j = i - 1;
                const auto arg = args[j];

                // mov first 4 args into calling registers
                if (j < CALL_REGISTERS.size())
                {
                    const Storage reg(CALL_REGISTERS[j]);
                    mov(arg, reg);
                }
                // push other args from right to left
                else
                {
                    displacement -= 8;
                    const Storage mem(0, displacement, RSP, NLL, 0);
                    mov(arg, mem);
                }
            }

            call(callee, dst);
        }
        return;
    case Instruction::Gep:
        {
            const auto base = value->GetOperand(0);
            const auto indices = value->GetOperandRange(1);
            auto type = base->GetType();

            const Storage rax(RAX);
            Print(base, rax);

            for (const auto index : indices)
            {
                if (const auto pt = dynamic_cast<PointerType*>(type))
                    type = pt->GetElementType();
                else if (const auto at = dynamic_cast<ArrayType*>(type))
                    type = at->GetElementType();
                else if (const auto st = dynamic_cast<StructType*>(type))
                {
                    const auto i = dynamic_cast<ConstantInt*>(index);
                    const auto ei = i->GetVal();
                    type = st->GetElementType(ei);

                    if (const uint64_t offset = st->GetElementOffset(ei))
                    {
                        const Storage imm(offset);
                        add(imm, rax, 8);
                    }
                    continue;
                }
                else Error("not an indexable type: {}", type);

                if (!index->NotNull()) continue;

                const uint64_t size = type->CountBytes();
                if (!size) continue;

                const Storage rcx(RCX);
                const Storage imm(size);
                mov(index, rcx);
                imul(imm, rcx, 8);
                add(rcx, rax, 8);
            }

            mov(rax, dst, 8);
        }
        return;
    case Instruction::Load:
        {
            const auto ptr = value->GetOperand(0);
            const auto bytes = value->GetType()->CountBytes();

            const Storage rax(RAX);
            const Storage storage(0, 0, RAX, NLL, 0);

            Print(ptr, rax);
            mov(storage, rax, bytes);
            mov(rax, dst, bytes);
        }
        return;
    case Instruction::Store:
        {
            const auto ptr = value->GetOperand(0);
            const auto val = value->GetOperand(1);
            const auto bytes = val->GetType()->CountBytes();

            const Storage rax(RAX);
            const Storage rcx(RCX);
            const Storage storage(0, 0, RCX, NLL, 0);

            Print(ptr, rcx);
            Print(val, rax);
            mov(rax, storage, bytes);
        }
        return;
    case Instruction::Alloca:
        {
            const auto bytes = value->CountAlloca();
            m_Offset -= bytes;
            const auto displacement = m_Offset;

            const Storage rbp(RBP);
            const Storage rax(RAX);
            const Storage imm(displacement);
            mov(rbp, rax, 8);
            add(imm, rax, 8);
            mov(rax, dst, 8);
        }
        return;
    case Instruction::Br:
        {
            const auto dst_val = value->GetOperand(0);

            jmp(dst_val);
        }
        return;
    case Instruction::Br_Lt:
        {
            const auto l_src = value->GetOperand(0);
            const auto r_src = value->GetOperand(1);
            const auto then_dst = value->GetOperand(2);
            const auto else_dst = value->GetOperand(3);

            const Storage tmp(RAX);
            mov(l_src, tmp);
            cmp(r_src, tmp);
            jl(then_dst);
            jmp(else_dst);
        }
        return;
    case Instruction::Ret:
        {
            const auto result = value->GetOperand(0);

            const Storage rax(RAX);
            const Storage rbp(RBP);
            const Storage rsp(RSP);

            Print(result, rax);

            mov(rbp, rsp, 8);
            pop(rbp, 8);
            ret();
        }
        return;

    default: break;
    }

    Error("X86Printer::Print(Instruction*) not implemented: {}", value);
}

void Brewer::X86Printer::Print(NamedValue* value, const Storage& dst)
{
    if (const auto ptr = dynamic_cast<FunctionBlock*>(value))
        return Print(ptr, dst);
    if (const auto ptr = dynamic_cast<GlobalValue*>(value))
        return Print(ptr, dst);

    const auto displacement = GetOffset(value);
    const auto bytes = value->GetType()->CountBytes();
    const Storage storage(0, displacement, RBP, NLL, 0);
    mov(storage, dst, bytes);
}

void Brewer::X86Printer::Print(FunctionBlock* value, const Storage& dst)
{
    lea(value, dst);
}

void Brewer::X86Printer::Print(GlobalValue* value, const Storage& dst)
{
    lea(value, dst);
}

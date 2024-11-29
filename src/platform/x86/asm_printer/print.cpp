#include <Brewer/Type.hpp>
#include <Brewer/Platform/X86/ASMPrinter.hpp>
#include <Brewer/Value/Constant.hpp>
#include <Brewer/Value/GlobalValue.hpp>
#include <Brewer/Value/NamedValue.hpp>
#include <Brewer/Value/Value.hpp>

void Brewer::Platform::X86::ASMPrinter::Print(Value* value, const Storage& dst)
{
    if (const auto ptr = dynamic_cast<Assignment*>(value))
        return Print(ptr);
    if (const auto ptr = dynamic_cast<Constant*>(value))
        return Print(ptr, dst);
    if (const auto ptr = dynamic_cast<Instruction*>(value))
        return Print(ptr, dst);
    if (const auto ptr = dynamic_cast<NamedValue*>(value))
        return Print(ptr, dst);

    Error("X86 - Print(Value*) not implemented: {}", value);
}

void Brewer::Platform::X86::ASMPrinter::Print(Assignment* value)
{
    const auto src = value->GetSrc();
    const auto dst = value->GetDst();
    const auto bytes = src->GetType()->GetNumBytes();
    const auto displacement = GetOffset(dst);

    const Storage rax(RA);
    const Storage storage(0, displacement, RBP, ZERO, 0);
    Print(src, rax);
    Mov(rax, storage, bytes);
}

void Brewer::Platform::X86::ASMPrinter::Print(Constant* value, const Storage& dst)
{
    if (const auto ptr = dynamic_cast<ConstantInt*>(value))
        return Print(ptr, dst);

    Error("X86 - Print(Constant*) not implemented: {}", value);
}

void Brewer::Platform::X86::ASMPrinter::Print(ConstantInt* value, const Storage& dst)
{
    const auto bytes = value->GetType()->GetNumBytes();
    const Storage imm(value->GetVal());
    Mov(imm, dst, bytes);
}

void Brewer::Platform::X86::ASMPrinter::Print(Instruction* value, const Storage& dst)
{
    switch (value->GetCode())
    {
    case Instruction::Add:
        {
            const auto l_src = value->GetOperand(0);
            const auto r_src = value->GetOperand(1);

            Mov(l_src, dst);
            Add(r_src, dst);
        }
        return;
    case Instruction::Sub:
        {
            const auto l_src = value->GetOperand(0);
            const auto r_src = value->GetOperand(1);

            Mov(l_src, dst);
            Sub(r_src, dst);
        }
        return;
    case Instruction::Call:
        {
            CallConv conv;
#ifdef _WIN32
            conv = MS_X64;
#else
            conv = SYSTEM_V;
#endif
            if (value->GetMeta("ms_x64")) conv = MS_X64;
            if (value->GetMeta("system_v")) conv = SYSTEM_V;

            const auto callee = value->GetOperand(0);
            const auto args = value->GetOperandRange(1);

            auto displacement = m_Top;
            for (unsigned i = args.size(); i > 0; --i)
            {
                const auto j = i - 1;
                const auto arg = args[j];

                if (j < GetNumCallRegisters(conv))
                {
                    const Storage reg(GetCallRegister(conv, j));
                    Mov(arg, reg);
                }
                else
                {
                    displacement -= 8;
                    const Storage mem(0, displacement, RSP, ZERO, 0);
                    Mov(arg, mem);
                }
            }

            Call(callee, dst);
        }
        return;
    case Instruction::Gep:
        {
            const auto base = value->GetOperand(0);
            const auto indices = value->GetOperandRange(1);
            auto type = base->GetType();

            const Storage rax(RA);
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
                        Add(imm, rax, 8);
                    }
                    continue;
                }
                else Error("not an indexable type: {}", type);

                if (!index->NotNull()) continue;

                const uint64_t size = type->GetNumBytes();
                if (!size) continue;

                const Storage rcx(RC);
                const Storage imm(size);
                Mov(index, rcx);
                IMul(imm, rcx, 8);
                Add(rcx, rax, 8);
            }

            Mov(rax, dst, 8);
        }
        return;
    case Instruction::Load:
        {
            const auto ptr = value->GetOperand(0);
            const auto bytes = value->GetType()->GetNumBytes();

            const Storage rax(RA);
            const Storage storage(0, 0, RA, ZERO, 0);

            Print(ptr, rax);
            Mov(storage, rax, bytes);
            Mov(rax, dst, bytes);
        }
        return;
    case Instruction::Store:
        {
            const auto ptr = value->GetOperand(0);
            const auto val = value->GetOperand(1);
            const auto bytes = val->GetType()->GetNumBytes();

            const Storage rax(RA);
            const Storage rcx(RC);
            const Storage storage(0, 0, RC, ZERO, 0);

            Print(ptr, rcx);
            Print(val, rax);
            Mov(rax, storage, bytes);
        }
        return;
    case Instruction::Alloca:
        {
            const auto bytes = value->GetNumAllocaBytes();
            m_Offset -= bytes;
            const auto displacement = m_Offset;

            const Storage rbp(RBP);
            const Storage rax(RA);
            const Storage imm(displacement);
            Mov(rbp, rax, 8);
            Add(imm, rax, 8);
            Mov(rax, dst, 8);
        }
        return;
    case Instruction::Br:
        {
            const auto dst_val = value->GetOperand(0);

            Jmp(dst_val);
        }
        return;
    case Instruction::Br_Lt:
        {
            const auto l_src = value->GetOperand(0);
            const auto r_src = value->GetOperand(1);
            const auto then_dst = value->GetOperand(2);
            const auto else_dst = value->GetOperand(3);

            const Storage tmp(RA);
            Mov(l_src, tmp);
            Cmp(r_src, tmp);
            Jl(then_dst);
            Jmp(else_dst);
        }
        return;
    case Instruction::Ret:
        {
            const auto result = value->GetOperand(0);

            const Storage rax(RA);
            const Storage rbp(RBP);
            const Storage rsp(RSP);

            Print(result, rax);

            Mov(rbp, rsp, 8);
            Pop(rbp, 8);
            Ret();
        }
        return;

    default: break;
    }

    Error("X86 - Print(Instruction*) not implemented: {}", value);
}

void Brewer::Platform::X86::ASMPrinter::Print(NamedValue* value, const Storage& dst)
{
    if (const auto ptr = dynamic_cast<FunctionBlock*>(value))
        return Print(ptr, dst);
    if (const auto ptr = dynamic_cast<GlobalValue*>(value))
        return Print(ptr, dst);

    const auto displacement = GetOffset(value);
    const auto bytes = value->GetType()->GetNumBytes();
    const Storage storage(0, displacement, RBP, ZERO, 0);
    Mov(storage, dst, bytes);
}

void Brewer::Platform::X86::ASMPrinter::Print(FunctionBlock* value, const Storage& dst)
{
    Lea(value, dst);
}

void Brewer::Platform::X86::ASMPrinter::Print(GlobalValue* value, const Storage& dst)
{
    Lea(value, dst);
}

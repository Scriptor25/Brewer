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

void Brewer::X86Printer::Print(Value* value, const Storage* dst)
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

    const RegisterStorage rax(RAX);
    const MemoryStorage storage(0, displacement, RBP, NLL, 0);
    Print(src, &rax);
    mov(&rax, &storage, bytes);
}

void Brewer::X86Printer::Print(Constant* value, const Storage* dst)
{
    if (const auto ptr = dynamic_cast<ConstantInt*>(value))
        return Print(ptr, dst);

    Error("X86Printer::Print(Constant*) not implemented: {}", value);
}

void Brewer::X86Printer::Print(ConstantInt* value, const Storage* dst)
{
    const auto bytes = value->GetType()->CountBytes();
    const ImmediateStorage imm(value->GetVal());
    mov(&imm, dst, bytes);
}

void Brewer::X86Printer::Print(Instruction* value, const Storage* dst)
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
            const auto callee = value->GetOperand(0);
            const auto args = value->GetOperandRange(1);

            for (unsigned i = 0; i < args.size() && i < CALL_REGISTERS.size(); ++i)
            {
                const RegisterStorage tmp(CALL_REGISTERS[i]);
                mov(args[i], &tmp);
            }
            for (unsigned i = args.size(); i > CALL_REGISTERS.size(); --i)
                push(args[i - 1]);

            call(callee, dst);
        }
        return;
    case Instruction::Gep:
        {
            const auto base = value->GetOperand(0);
            const auto indices = value->GetOperandRange(1);
            auto type = base->GetType();

            const RegisterStorage rax(RAX);
            Print(base, &rax);

            for (unsigned i = 0; i < indices.size(); ++i)
            {
                if (const auto pt = dynamic_cast<PointerType*>(type))
                    type = pt->GetElementType();
                else if (const auto at = dynamic_cast<ArrayType*>(type))
                    type = at->GetElementType();
                else if (const auto st = dynamic_cast<StructType*>(type))
                {
                    const auto index = dynamic_cast<ConstantInt*>(indices[i]);
                    const auto ei = index->GetVal();
                    type = st->GetElementType(ei);

                    if (const auto offset = st->GetElementOffset(ei))
                    {
                        const ImmediateStorage imm(offset);
                        add(&imm, &rax, 8);
                    }
                    continue;
                }
                else Error("not an indexable type: {}", type);

                const auto index = indices[i];
                if (!index->NotNull()) continue;

                const auto size = static_cast<int>(type->CountBytes());
                if (!size) continue;

                const RegisterStorage rcx(RCX);
                const ImmediateStorage imm(size);
                mov(index, &rcx);
                imul(&imm, &rcx, 8);
                add(&rcx, &rax, 8);
            }

            mov(&rax, dst, 8);
        }
        return;
    case Instruction::Load:
        {
            const auto ptr = value->GetOperand(0);
            const auto bytes = value->GetType()->CountBytes();

            const RegisterStorage rax(RAX);
            const MemoryStorage storage(0, 0, RAX, NLL, 0);

            Print(ptr, &rax);
            mov(&storage, &rax, bytes);
            mov(&rax, dst, bytes);
        }
        return;
    case Instruction::Store:
        {
            const auto ptr = value->GetOperand(0);
            const auto val = value->GetOperand(1);
            const auto bytes = val->GetType()->CountBytes();

            const RegisterStorage rax(RAX);
            const RegisterStorage rcx(RCX);
            const MemoryStorage storage(0, 0, RCX, NLL, 0);

            Print(ptr, &rcx);
            Print(val, &rax);
            mov(&rax, &storage, bytes);
        }
        return;
    case Instruction::Alloca:
        {
            const auto type = value->GetType()->GetElementType();

            const auto count = dynamic_cast<ConstantInt*>(value->GetOperand(0))->GetVal();
            auto bytes = type->CountBytes() * count;
            if (const auto rem = bytes % 0x10)
                bytes += 0x10 - rem;

            const ImmediateStorage imm(static_cast<int>(bytes));
            const RegisterStorage rsp(RSP);
            sub(&imm, &rsp, 8);
            mov(&rsp, dst, 8);
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

            const RegisterStorage tmp(RAX);
            mov(l_src, &tmp);
            cmp(r_src, &tmp);
            jl(then_dst);
            jmp(else_dst);
        }
        return;
    case Instruction::Ret:
        {
            const auto result = value->GetOperand(0);

            const RegisterStorage rax(RAX);
            const RegisterStorage rbp(RBP);
            const RegisterStorage rsp(RSP);

            Print(result, &rax);

            mov(&rbp, &rsp, 8);
            pop(&rbp, 8);
            ret();
        }
        return;

    default: break;
    }

    Error("X86Printer::Print(Instruction*) not implemented: {}", value);
}

void Brewer::X86Printer::Print(NamedValue* value, const Storage* dst)
{
    if (const auto ptr = dynamic_cast<FunctionBlock*>(value))
        return Print(ptr, dst);
    if (const auto ptr = dynamic_cast<GlobalValue*>(value))
        return Print(ptr, dst);

    const auto displacement = GetOffset(value);
    const auto bytes = value->GetType()->CountBytes();
    const MemoryStorage storage(0, displacement, RBP, NLL, 0);
    mov(&storage, dst, bytes);
}

void Brewer::X86Printer::Print(FunctionBlock* value, const Storage* dst)
{
    lea(value, dst);
}

void Brewer::X86Printer::Print(GlobalValue* value, const Storage* dst)
{
    lea(value, dst);
}

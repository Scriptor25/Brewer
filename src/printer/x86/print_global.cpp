#include <Brewer/Type.hpp>
#include <Brewer/Printer/X86Printer.hpp>
#include <Brewer/Value/Constant.hpp>
#include <Brewer/Value/GlobalValue.hpp>
#include <Brewer/Value/NamedValue.hpp>

void Brewer::X86Printer::PrintGlobal(Value* value)
{
    if (const auto ptr = dynamic_cast<NamedValue*>(value))
        return PrintGlobal(ptr);

    Error("X86Printer::PrintGlobal(Value*) not implemented: {}", value);
}

void Brewer::X86Printer::PrintGlobal(NamedValue* value)
{
    if (const auto ptr = dynamic_cast<GlobalValue*>(value))
        return PrintGlobal(ptr);
    if (const auto ptr = dynamic_cast<FunctionBlock*>(value))
        return PrintGlobal(ptr);

    Error("X86Printer::PrintGlobal(NamedValue*) not implemented: {}", value);
}

void Brewer::X86Printer::PrintGlobal(GlobalValue* value)
{
    if (const auto ptr = dynamic_cast<GlobalVariable*>(value))
        return PrintGlobal(ptr);
    if (const auto ptr = dynamic_cast<GlobalFunction*>(value))
        return PrintGlobal(ptr);

    Error("X86Printer::PrintGlobal(GlobalValue*) not implemented: {}", value);
}

void Brewer::X86Printer::PrintGlobal(GlobalVariable* value)
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
    PrintType(value->GetType()->GetBaseType());
    S() << ' ';
    PrintGlobalOperand(value->GetInit());
    S() << std::endl;
}

void Brewer::X86Printer::PrintGlobal(GlobalFunction* value)
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

    BeginFrame();

    const RegisterStorage rbp(RBP);
    const RegisterStorage rsp(RSP);

    push(&rbp, 8);
    mov(&rsp, &rbp, 8);

    if (auto bytes = value->GetByteAlloc())
    {
        if (const auto rem = bytes % 0x10)
            bytes += 0x10 - rem;
        const ImmediateStorage imm(static_cast<int>(bytes));
        sub(&imm, &rsp, 8);
    }

    for (unsigned i = 0; i < value->GetNumArgs(); ++i)
    {
        const auto arg = value->GetArg(i);
        const auto offset = 0x10 + 8 * static_cast<int>(i);
        m_Offsets[arg] = offset;
        if (i < CALL_REGISTERS.size())
        {
            const auto bytes = arg->GetType()->CountBytes();
            const RegisterStorage reg(CALL_REGISTERS[i]);
            const MemoryStorage storage(0, offset, RBP, NLL, 0);
            mov(&reg, &storage, bytes);
        }
    }

    for (unsigned i = 0; i < value->GetNumBlocks(); ++i)
        PrintGlobal(value->GetBlock(i));
}

void Brewer::X86Printer::PrintGlobal(FunctionBlock* value)
{
    if (!value->GetName().empty())
        S() << ".L" << value->GetIndex() << ':' << std::endl;
    for (unsigned i = 0; i < value->GetNumValues(); ++i)
        Print(value->GetValue(i));
}

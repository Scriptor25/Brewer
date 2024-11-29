#include <Brewer/Type.hpp>
#include <Brewer/Platform/X86/ASMPrinter.hpp>
#include <Brewer/Value/Constant.hpp>
#include <Brewer/Value/GlobalValue.hpp>
#include <Brewer/Value/NamedValue.hpp>

void Brewer::Platform::X86::ASMPrinter::PrintGlobal(Value* value)
{
    if (const auto ptr = dynamic_cast<NamedValue*>(value))
        return PrintGlobal(ptr);

    Error("X86 - PrintGlobal(Value*) not implemented: {}", value);
}

void Brewer::Platform::X86::ASMPrinter::PrintGlobal(NamedValue* value)
{
    if (const auto ptr = dynamic_cast<GlobalValue*>(value))
        return PrintGlobal(ptr);
    if (const auto ptr = dynamic_cast<FunctionBlock*>(value))
        return PrintGlobal(ptr);

    Error("X86 - PrintGlobal(NamedValue*) not implemented: {}", value);
}

void Brewer::Platform::X86::ASMPrinter::PrintGlobal(GlobalValue* value)
{
    if (const auto ptr = dynamic_cast<GlobalVariable*>(value))
        return PrintGlobal(ptr);
    if (const auto ptr = dynamic_cast<GlobalFunction*>(value))
        return PrintGlobal(ptr);

    Error("X86 - PrintGlobal(GlobalValue*) not implemented: {}", value);
}

void Brewer::Platform::X86::ASMPrinter::PrintGlobal(GlobalVariable* value)
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

void Brewer::Platform::X86::ASMPrinter::PrintGlobal(GlobalFunction* value)
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

    const Storage rbp(RBP);
    const Storage rsp(RSP);

    Push(rbp, 8);
    Mov(rsp, rbp, 8);

    if (uint64_t bytes = value->GetNumAllocaBytes())
    {
        if (const auto rem = bytes % 0x10)
            bytes += 0x10 - rem;

        m_TopOffset = static_cast<int64_t>(bytes);

        const Storage imm(bytes);
        Sub(imm, rsp, 8);
    }

    CallConv conv;
#ifdef _WIN32
    conv = MS_X64;
#else
    conv = SYSTEM_V;
#endif
    if (value->GetMeta("ms_x64")) conv = MS_X64;
    if (value->GetMeta("system_v")) conv = SYSTEM_V;

    for (unsigned i = 0; i < value->GetNumArgs(); ++i)
    {
        const auto arg = value->GetArg(i);
        const auto offset = 0x10 + 8 * i;
        m_Offsets[arg] = offset;
        if (i < GetNumCallRegisters(conv))
        {
            const auto bytes = arg->GetType()->GetNumBytes();
            const Storage reg(GetCallRegister(conv, i));
            const Storage storage(0, offset, RBP, ZERO, 0);
            Mov(reg, storage, bytes);
        }
    }

    for (unsigned i = 0; i < value->GetNumBlocks(); ++i)
        PrintGlobal(value->GetBlock(i));
}

void Brewer::Platform::X86::ASMPrinter::PrintGlobal(FunctionBlock* value)
{
    if (!value->GetName().empty())
        S() << ".L" << value->GetIndex() << ':' << std::endl;
    for (unsigned i = 0; i < value->GetNumValues(); ++i)
        Print(value->GetValue(i));
}

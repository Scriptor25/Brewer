#include <Brewer/Module.hpp>
#include <Brewer/Type.hpp>
#include <Brewer/Printer/X86Printer.hpp>
#include <Brewer/Value/Block.hpp>
#include <Brewer/Value/Constant.hpp>
#include <Brewer/Value/Function.hpp>
#include <Brewer/Value/GlobalValue.hpp>
#include <Brewer/Value/GlobalVariable.hpp>
#include <Brewer/Value/Instruction.hpp>
#include <Brewer/Value/Value.hpp>

Brewer::X86Printer::X86Printer(std::ostream& stream)
    : Printer(stream)
{
}

void Brewer::X86Printer::Print(Module* module)
{
    module->ForEach([&](GlobalValue* value)
    {
        Print(value);
    });
}

void Brewer::X86Printer::Print(Value* value)
{
    if (const auto ptr = dynamic_cast<Constant*>(value))
        return Print(ptr);
    if (const auto ptr = dynamic_cast<NamedValue*>(value))
        return Print(ptr);

    Error("X86Printer::Print(Value*) not implemented: {}", value);
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
    switch (type->Bits())
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
    Error("X86Printer::Print(FloatType*) not implemented: {}", type);
}

void Brewer::X86Printer::Print(PointerType*)
{
    S() << ".quad";
}

void Brewer::X86Printer::Print(ArrayType* type)
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
    Print(value->GetType<ArrayType>()->GetElementType());
    S() << ' ';
    for (unsigned i = 0; i < value->GetNumElements(); ++i)
    {
        if (i > 0) S() << ", ";
        PrintGlobalOperand(value->GetElement(i));
    }
    S() << std::endl;
}

void Brewer::X86Printer::Print(NamedValue* value)
{
    if (const auto ptr = dynamic_cast<GlobalValue*>(value))
        return Print(ptr);
    if (const auto ptr = dynamic_cast<Block*>(value))
        return Print(ptr);
    if (const auto ptr = dynamic_cast<Instruction*>(value))
        return Print(ptr);

    Error("X86Printer::Print(NamedValue*) not implemented: {}", value);
}

void Brewer::X86Printer::Print(GlobalValue* value)
{
    if (const auto ptr = dynamic_cast<GlobalVariable*>(value))
        return Print(ptr);
    if (const auto ptr = dynamic_cast<Function*>(value))
        return Print(ptr);

    Error("X86Printer::Print(GlobalValue*) not implemented: {}", value);
}

void Brewer::X86Printer::Print(GlobalVariable* value)
{
    S() << ".section .data" << std::endl;
    switch (value->GetLinkage())
    {
    case GlobalValue::ExternLinkage:
        S() << ".global " << value->GetName() << std::endl;
        break;
    case GlobalValue::InternLinkage:
        S() << ".local " << value->GetName() << std::endl;
        break;
    case GlobalValue::WeakLinkage:
        S() << ".weak " << value->GetName() << std::endl;
        break;

    case GlobalValue::NoLinkage:
    case GlobalValue::CommonLinkage:
    case GlobalValue::TentativeLinkage:
    default: break;
    }

    if (!value->GetInitializer())
    {
        S() << ".extern " << value->GetName() << std::endl;
        return;
    }

    S() << value->GetName() << ": ";
    Print(value->GetInitializer()->GetType());
    S() << ' ';
    PrintGlobalOperand(value->GetInitializer());
    S() << std::endl;

    Print(value->GetInitializer());
}

void Brewer::X86Printer::Print(Function* value)
{
    S() << ".section .text" << std::endl;
    switch (value->GetLinkage())
    {
    case GlobalValue::ExternLinkage:
        S() << ".global " << value->GetName() << std::endl;
        break;
    case GlobalValue::InternLinkage:
        S() << ".local " << value->GetName() << std::endl;
        break;
    case GlobalValue::WeakLinkage:
        S() << ".weak " << value->GetName() << std::endl;
        break;

    case GlobalValue::NoLinkage:
    case GlobalValue::CommonLinkage:
    case GlobalValue::TentativeLinkage:
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

    for (unsigned i = 0; i < value->GetNumBlocks(); ++i)
        Print(value->GetBlock(i));
}

void Brewer::X86Printer::Print(Block* value)
{
    S() << ".L" << value->GetIndex() << ':' << std::endl;
    for (unsigned i = 0; i < value->GetNumValues(); ++i)
        Print(value->GetValue(i));
}

void Brewer::X86Printer::Print(Instruction* value)
{
    switch (value->GetCode())
    {
    case Instruction::Call:
        {
            const auto callee = value->GetOperand(0);
            const auto args = value->GetSubOperands(1);

            for (unsigned i = 0; i < args.size(); ++i)
            {
                S() << "mov";
                PrintOperand(args[i]);
            }

            S() << "call ";
            PrintCallee(callee);
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

void Brewer::X86Printer::PrintCallee(Value* value)
{
    Error("X86Printer::PrintCallee(Value*) not implemented: {}", value);
}

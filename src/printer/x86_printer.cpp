#include <Brewer/Module.hpp>
#include <Brewer/Type.hpp>
#include <Brewer/Printer/X86Printer.hpp>
#include <Brewer/Value/Block.hpp>
#include <Brewer/Value/Constant.hpp>
#include <Brewer/Value/Function.hpp>
#include <Brewer/Value/GlobalValue.hpp>
#include <Brewer/Value/GlobalVariable.hpp>
#include <Brewer/Value/Instruction.hpp>

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

    default:
        break;
    }

    Error("X86Printer::Print(IntType*) not implemented: {}", type);
}

void Brewer::X86Printer::Print(FloatType* type)
{
    Error("X86Printer::Print(FloatType*) not implemented: {}", type);
}

void Brewer::X86Printer::Print(PointerType* type)
{
    Error("X86Printer::Print(PointerType*) not implemented: {}", type);
}

void Brewer::X86Printer::Print(ArrayType* type)
{
    Print(type->GetElementType());
}

void Brewer::X86Printer::Print(Constant* value)
{
    if (const auto ptr = dynamic_cast<ConstantInt*>(value))
        return Print(ptr);
    if (const auto ptr = dynamic_cast<ConstantFloat*>(value))
        return Print(ptr);
    if (const auto ptr = dynamic_cast<ConstantArray*>(value))
        return Print(ptr);

    Error("X86Printer::Print(Constant*) not implemented: {}", value);
}

void Brewer::X86Printer::Print(ConstantInt* value)
{
    if (m_Printed[value]) return;
    m_Printed[value] = true;

    Error("X86Printer::Print(ConstantInt*) not implemented: {}", value);
}

void Brewer::X86Printer::Print(ConstantFloat* value)
{
    if (m_Printed[value]) return;
    m_Printed[value] = true;

    Error("X86Printer::Print(ConstantFloat*) not implemented: {}", value);
}

void Brewer::X86Printer::Print(ConstantArray* value)
{
    if (m_Printed[value]) return;
    m_Printed[value] = true;

    S() << ".section .rodata" << std::endl;
    S() << ".LC" << value->GetIndex() << ": ";
    Print(value->GetType());
    S() << ' ';
    for (unsigned i = 0; i < value->GetNumElements(); ++i)
    {
        if (i > 0) S() << ", ";
        PrintOperand(value->GetElement(i));
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
    if (m_Printed[value]) return;
    m_Printed[value] = true;

    Print(value->GetInitializer());
    S() << ".section .data" << std::endl;
    switch (value->GetLinkage())
    {
    case GlobalValue::ExternLinkage:
        S() << ".global ";
        break;
    case GlobalValue::InternLinkage:
        S() << ".local ";
        break;
    case GlobalValue::WeakLinkage:
        S() << ".weak ";
        break;
    case GlobalValue::TentativeLinkage:
    case GlobalValue::CommonLinkage:
    case GlobalValue::NoLinkage:
    default: break;
    }
    S() << value->GetName() << ": ";
    PrintOperand(value->GetInitializer());
    S() << std::endl;
}

void Brewer::X86Printer::Print(Function* value)
{
    if (m_Printed[value]) return;
    m_Printed[value] = true;

    if (value->IsEmpty())
        return;

    S() << ".section .text" << std::endl;
    switch (value->GetLinkage())
    {
    case GlobalValue::ExternLinkage:
        S() << ".global ";
        break;
    case GlobalValue::InternLinkage:
        S() << ".local ";
        break;
    case GlobalValue::WeakLinkage:
        S() << ".weak ";
        break;

    case GlobalValue::TentativeLinkage:
    case GlobalValue::CommonLinkage:
    case GlobalValue::NoLinkage:
    default: break;
    }

    S() << value->GetName() << ':' << std::endl;

    S() << "pushq %rbp" << std::endl;
    S() << "movq %rsp, %rbp" << std::endl;

    for (unsigned i = 0; i < value->GetNumBlocks(); ++i)
        Print(value->GetBlock(i));
}

void Brewer::X86Printer::Print(Block* value)
{
    if (m_Printed[value]) return;
    m_Printed[value] = true;

    if (!value->GetName().empty())
        S() << ".L" << value->GetIndex() << ':' << std::endl;

    for (unsigned i = 0; i < value->GetNumValues(); ++i)
    {
        Print(value->GetValue(i));
        S() << std::endl;
    }
}

void Brewer::X86Printer::Print(Instruction* value)
{
    if (m_Printed[value]) return;
    m_Printed[value] = true;
    switch (value->GetCode())
    {
    case Instruction::ICmpLT:
        break;
    case Instruction::ICmpLE:
        break;
    case Instruction::IAdd:
        break;
    case Instruction::ISub:
        break;
    case Instruction::PtrCast:
        break;
    case Instruction::Call:
        S() << "call ";
        PrintOperand(value->GetOperand(0));
        S() << std::endl;
        return;
    case Instruction::GEP:
        break;
    case Instruction::PHI:
        break;
    case Instruction::Ret:
        break;
    case Instruction::Br:
        break;
    }

    Error("X86Printer::Print(Instruction*) not implemented: {}", value);
}

void Brewer::X86Printer::PrintOperand(Value* value)
{
    if (const auto ptr = dynamic_cast<Constant*>(value))
        return PrintOperand(ptr);
    if (const auto ptr = dynamic_cast<NamedValue*>(value))
        return PrintOperand(ptr);

    Error("X86Printer::PrintOperand(Value*) not implemented: {}", value);
}

void Brewer::X86Printer::PrintOperand(Constant* value)
{
    if (const auto ptr = dynamic_cast<ConstantInt*>(value))
        return PrintOperand(ptr);
    if (const auto ptr = dynamic_cast<ConstantFloat*>(value))
        return PrintOperand(ptr);
    if (const auto ptr = dynamic_cast<ConstantArray*>(value))
        return PrintOperand(ptr);

    Error("X86Printer::PrintOperand(Constant*) not implemented: {}", value);
}

void Brewer::X86Printer::PrintOperand(ConstantInt* value)
{
    S() << value->GetVal();
}

void Brewer::X86Printer::PrintOperand(ConstantFloat* value)
{
    Error("X86Printer::PrintOperand(ConstantFloat*) not implemented: {}", value);
}

void Brewer::X86Printer::PrintOperand(ConstantArray* value)
{
    S() << ".quad .LC" << value->GetIndex();
}

void Brewer::X86Printer::PrintOperand(NamedValue* value)
{
    if (const auto ptr = dynamic_cast<GlobalValue*>(value))
        return PrintOperand(ptr);

    Error("X86Printer::PrintOperand(NamedValue*) not implemented: {}", value);
}

void Brewer::X86Printer::PrintOperand(GlobalValue* value)
{
    if (const auto ptr = dynamic_cast<Function*>(value))
        return PrintOperand(ptr);

    Error("X86Printer::PrintOperand(GlobalValue*) not implemented: {}", value);
}

void Brewer::X86Printer::PrintOperand(Function* value)
{
    S() << value->GetName();
}

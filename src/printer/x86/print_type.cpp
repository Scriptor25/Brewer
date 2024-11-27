#include <Brewer/Type.hpp>
#include <Brewer/Printer/X86Printer.hpp>

void Brewer::X86Printer::PrintType(Type* type)
{
    if (const auto ptr = dynamic_cast<IntType*>(type))
        return PrintType(ptr);
    if (const auto ptr = dynamic_cast<FloatType*>(type))
        return PrintType(ptr);
    if (const auto ptr = dynamic_cast<PointerType*>(type))
        return PrintType(ptr);
    if (const auto ptr = dynamic_cast<ArrayType*>(type))
        return PrintType(ptr);

    Error("X86Printer::Print(Type*) not implemented: {}", type);
}

void Brewer::X86Printer::PrintType(IntType* type)
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

void Brewer::X86Printer::PrintType(FloatType* type)
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

void Brewer::X86Printer::PrintType(PointerType*)
{
    S() << ".quad";
}

void Brewer::X86Printer::PrintType(ArrayType*)
{
    S() << ".quad";
}

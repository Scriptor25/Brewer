#include <Brewer/Type.hpp>
#include <Brewer/Platform/X86/ASMPrinter.hpp>

void Brewer::Platform::X86::ASMPrinter::PrintType(Type *type) const
{
    if (const auto ptr = dynamic_cast<IntType *>(type))
        return PrintType(ptr);
    if (const auto ptr = dynamic_cast<FloatType *>(type))
        return PrintType(ptr);
    if (const auto ptr = dynamic_cast<PointerType *>(type))
        return PrintType(ptr);
    if (const auto ptr = dynamic_cast<ArrayType *>(type))
        return PrintType(ptr);

    Error("X86 - Print(Type*) not implemented: {}", type);
}

void Brewer::Platform::X86::ASMPrinter::PrintType(IntType *type) const
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

    Error("X86 - Print(IntType*) not implemented: {}", type);
}

void Brewer::Platform::X86::ASMPrinter::PrintType(FloatType *type) const
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

    Error("X86 - Print(FloatType*) not implemented: {}", type);
}

void Brewer::Platform::X86::ASMPrinter::PrintType(PointerType *) const
{
    S() << ".quad";
}

void Brewer::Platform::X86::ASMPrinter::PrintType(ArrayType *) const
{
    S() << ".quad";
}

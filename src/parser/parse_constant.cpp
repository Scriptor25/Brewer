#include <Brewer/Parser.hpp>
#include <Brewer/Type.hpp>
#include <Brewer/Value/Constant.hpp>

Brewer::Constant* Brewer::Parser::ParseConstant(Type* type)
{
    if (At(IntToken))
    {
        const auto int_type = dynamic_cast<IntType*>(type);
        if (!int_type) Error("constant int must have int type");
        return new ConstantInt(int_type, Skip().IntValue);
    }
    if (At(FloatToken))
    {
        const auto float_type = dynamic_cast<FloatType*>(type);
        if (!float_type) Error("constant float must have float type");
        return new ConstantFloat(float_type, Skip().FloatValue);
    }
    if (At(StringToken))
    {
        const auto str = Skip().Value;

        const auto array_type = dynamic_cast<ArrayType*>(type);
        if (!array_type) Error("constant string must have array type");
        const auto size = array_type->Size();
        if (size != str.size()) Error("constant string size must equal array size");
        const auto element_type = dynamic_cast<IntType*>(array_type->GetElementType());
        if (!element_type) Error("constant string type must have int element type");
        if (element_type->Bits() != 8) Error("constant string element type must be of size 8 bits");

        std::vector<Constant*> elements(size);
        for (unsigned i = 0; i < size; ++i)
            elements[i] = new ConstantInt(element_type, str[i]);
        return new ConstantArray(array_type, elements);
    }
    if (NextAt("["))
    {
        const auto array_type = dynamic_cast<ArrayType*>(type);
        const auto size = array_type->Size();
        const auto element_type = array_type->GetElementType();

        std::vector<Constant*> elements(size);
        for (unsigned i = 0; i < size; ++i)
        {
            if (i > 0) Expect(",");
            const auto el_ty = ParseType();
            if (el_ty != element_type)
                Error("type mismatch: {} != {}", el_ty, element_type);
            elements[i] = ParseConstant(el_ty);
        }
        Expect("]");

        return new ConstantArray(array_type, elements);
    }

    Error("expected constant");
}

#include <Brewer/Context.hpp>
#include <Brewer/Module.hpp>
#include <Brewer/Parser.hpp>
#include <Brewer/Type.hpp>
#include <Brewer/Value/Constant.hpp>

Brewer::ConstantArray* Brewer::Parser::ParseConstantArray(Type* type)
{
    const auto array_type = dynamic_cast<ArrayType*>(type);
    if (!array_type)
        Error("not an array type");

    std::vector<Constant*> vals;

    if (At(TokenType_String))
    {
        const auto int_type = dynamic_cast<IntType*>(array_type->GetElementType());
        if (!int_type)
            Error("not an integer type");
        for (const auto str = Skip().Str; const auto c : str)
            vals.push_back(new ConstantInt(int_type, c));
    }
    else
    {
        Expect("[");
        while (!At("]"))
        {
            vals.push_back(ParseConstant(array_type->GetElementType()));
            if (!At("]"))
                Expect(",");
        }
        Expect("]");
    }

    if (array_type->GetNumElements() != vals.size())
        Error("array size mismatch");

    return new ConstantArray(array_type, std::move(vals));
}

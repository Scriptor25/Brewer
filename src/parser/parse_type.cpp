#include <vector>
#include <Brewer/Context.hpp>
#include <Brewer/Module.hpp>
#include <Brewer/Parser.hpp>

Brewer::Type* Brewer::Parser::ParseType()
{
    Type* result_type{};

    if (NextAt("["))
    {
        const auto element_type = ParseType();
        if (NextAt("x"))
        {
            const auto size = Expect(IntToken).IntValue;
            result_type = m_Dest.GetContext().GetArrayType(element_type, size);
        }
        else result_type = m_Dest.GetContext().GetPointerType(element_type);
        Expect("]");
    }
    else if (NextAt("{"))
    {
        std::vector<Type*> element_types;

        while (!At("}") && !AtEof())
        {
            element_types.push_back(ParseType());
            if (!At("}"))
                Expect(",");
        }
        Expect("}");

        result_type = m_Dest.GetContext().GetStructType(element_types);
    }
    else

        if (!result_type) Error("expected a type");

    while (NextAt("("))
    {
        std::vector<Type*> arg_types;
        bool vararg = false;

        while (!At(")") && !AtEof())
        {
            if (NextAt("."))
            {
                Expect(".");
                Expect(".");
                vararg = true;
                break;
            }

            arg_types.push_back(ParseType());

            if (!At(")"))
                Expect(",");
        }
        Expect(")");

        result_type = m_Dest.GetContext().GetFunctionType(result_type, arg_types, vararg);
    }

    return result_type;
}

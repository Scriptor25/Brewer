#include <vector>
#include <Brewer/Context.hpp>
#include <Brewer/Module.hpp>
#include <Brewer/Parser.hpp>
#include <Brewer/Type.hpp>

Brewer::Type *Brewer::Parser::ParseType()
{
    Type *result_type{};

    if (NextAt("["))
    {
        const auto element_type = ParseType();
        if (NextAt("x"))
        {
            const auto size = Expect(TokenType_Int).Int;
            result_type = m_Dest.GetContext().GetArrayType(element_type, size);
        }
        else
            result_type = m_Dest.GetContext().GetPointerType(element_type);
        Expect("]");
    }
    else if (NextAt("{"))
    {
        std::vector<Type *> element_types;

        while (!At("}") && !AtEof())
        {
            element_types.push_back(ParseType());
            if (!At("}"))
                Expect(",");
        }
        Expect("}");

        result_type = m_Dest.GetContext().GetStructType(std::move(element_types));
    }
    else if (At(TokenType_Id))
    {
        if (const auto name = Skip().Str; name == "void")
            result_type = m_Dest.GetContext().GetVoidType();
        else if (name.front() == 'i')
        {
            const auto bits = std::stoi(name.substr(1));
            result_type = m_Dest.GetContext().GetIntNType(bits);
        }
        else if (name.front() == 'f')
        {
            const auto bits = std::stoi(name.substr(1));
            result_type = m_Dest.GetContext().GetFloatNType(bits);
        }
        else
            result_type = m_Dest.GetContext().GlobalType(name);
    }

    if (!result_type)
        Error("expected type");

    while (NextAt("("))
    {
        std::vector<Type *> arg_types;
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

        result_type = m_Dest.GetContext().GetFunctionType(result_type, std::move(arg_types), vararg);
    }

    return result_type;
}

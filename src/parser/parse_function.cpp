#include <vector>
#include <Brewer/Context.hpp>
#include <Brewer/Module.hpp>
#include <Brewer/Parser.hpp>
#include <Brewer/Value/Argument.hpp>
#include <Brewer/Value/Function.hpp>
#include <Brewer/Value/GlobalValue.hpp>
#include <Brewer/Value/Instruction.hpp>

Brewer::Function* Brewer::Parser::ParseFunction()
{
    const auto define = NextAt("define");
    if (!define) Expect("declare");
    const auto linkage = ToLinkage(Expect(IdToken).Value);
    const auto result_type = ParseType();
    const auto name = Expect(GlobalToken).Value;

    std::vector<Argument*> args;
    std::vector<Type*> arg_types;
    bool vararg = false;

    Expect("(");
    while (!At(")") && !AtEof())
    {
        if (NextAt("."))
        {
            Expect(".");
            Expect(".");
            vararg = true;
            break;
        }

        const auto arg_type = ParseType();
        const auto arg_name = Expect(LocalToken).Value;
        args.push_back(new Argument(arg_type, arg_name, args.size()));
        arg_types.push_back(arg_type);

        if (!At(")"))
            Expect(",");
    }
    Expect(")");

    const auto function_type = m_Dest.GetContext().GetFunctionType(result_type, arg_types, vararg);
    const auto function = new Function(function_type, name, linkage, args);
    m_Dest.SetGlobalValue(name, function);
    m_Parent = function;

    if (define)
    {
        Expect("{");
        while (!At("}") && !AtEof())
            function->Append(ParseNamedValue());
        Expect("}");
    }

    m_Parent = {};
    return function;
}

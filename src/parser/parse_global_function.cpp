#include <Brewer/Context.hpp>
#include <Brewer/Module.hpp>
#include <Brewer/Parser.hpp>
#include <Brewer/Value/GlobalValue.hpp>

Brewer::GlobalFunction* Brewer::Parser::ParseGlobalFunction()
{
    Expect("fun");

    const auto result_type = ParseType();
    GlobalValue::Linkage linkage = GlobalValue::Linkage_Global;
    if (!At(TokenType_GlobalId))
        linkage = ToLinkage(Expect(TokenType_Id).Str);
    auto name = Expect(TokenType_GlobalId).Str;

    std::vector<Type*> arg_types;
    std::vector<FunctionArg*> args;
    bool vararg = false;

    Expect("(");
    while (!At(")"))
    {
        if (NextAt("."))
        {
            Expect(".");
            Expect(".");
            vararg = true;
            break;
        }

        const auto arg_type = ParseType();
        arg_types.push_back(arg_type);
        const auto arg_name = Expect(TokenType_LocalId).Str;
        args.push_back(new FunctionArg(arg_type, arg_name));

        if (!At(")"))
            Expect(",");
    }
    Expect(")");

    const auto type = m_Dest.GetContext().GetFunctionType(result_type, std::move(arg_types), vararg);
    const auto function = new GlobalFunction(type, std::move(name), linkage, std::move(args));

    if (!NextAt("{"))
        return function;

    m_Parent = function;
    while (!At("}"))
        function->Append(ParseValue({}));
    Expect("}");
    m_Parent = {};

    return function;
}

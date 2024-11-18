#include <vector>
#include <Brewer/Parser.hpp>
#include <Brewer/Value/Argument.hpp>
#include <Brewer/Value/GlobalValue.hpp>

/**
 * <define|declare> [<linkage>] @<name>(<arg>...) [{...}]
 */
void Brewer::Parser::ParseFunction()
{
    const auto define = NextAt("define");
    if (!define) Expect("declare");

    GlobalValue::LinkageType linkage = GlobalValue::ExternalLinkage;
    if (NextAt("none")) linkage = GlobalValue::NoLinkage;
    else if (NextAt("external")) linkage = GlobalValue::ExternalLinkage;
    else if (NextAt("internal")) linkage = GlobalValue::InternalLinkage;
    else if (NextAt("weak")) linkage = GlobalValue::WeakLinkage;
    else if (NextAt("common")) linkage = GlobalValue::CommonLinkage;
    else if (NextAt("tentative")) linkage = GlobalValue::TentativeLinkage;

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

    if (define)
    {
        Expect("{");
        while (!At("}") && !AtEof())
            NextToken();
        Expect("}");
    }
}

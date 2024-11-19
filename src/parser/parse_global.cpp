#include <Brewer/Module.hpp>
#include <Brewer/Parser.hpp>
#include <Brewer/Value/GlobalValue.hpp>
#include <Brewer/Value/GlobalVariable.hpp>

Brewer::GlobalVariable* Brewer::Parser::ParseGlobal()
{
    Expect("global");
    const auto linkage = ToLinkage(Expect(IdToken).Value);
    const auto name = Expect(GlobalToken).Value;
    Expect("=");
    const auto type = ParseType();
    const auto initializer = ParseConstant(type);

    const auto variable = new GlobalVariable(type, name, linkage, initializer);
    m_Dest.SetGlobalValue(name, variable);
    return variable;
}

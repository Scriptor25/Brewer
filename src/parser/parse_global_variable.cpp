#include <Brewer/Parser.hpp>
#include <Brewer/Value/GlobalValue.hpp>

Brewer::GlobalVariable* Brewer::Parser::ParseGlobalVariable()
{
    Expect("var");
    GlobalValue::Linkage linkage = GlobalValue::Linkage_Local;
    if (!At(TokenType_GlobalId))
        linkage = ToLinkage(Expect(TokenType_Id).Str);
    auto name = Expect(TokenType_GlobalId).Str;
    Expect("=");
    const auto type = ParseType();
    const auto init = ParseConstant(type);
    return new GlobalVariable(type, std::move(name), linkage, init);
}
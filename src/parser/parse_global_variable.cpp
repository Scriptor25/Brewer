#include <Brewer/Parser.hpp>
#include <Brewer/Value/GlobalValue.hpp>

Brewer::GlobalVariable* Brewer::Parser::ParseGlobalVariable()
{
    Expect("var");
    GlobalValue::Linkage linkage = GlobalValue::Linkage_Local;
    if (!At(TokenType_GlobalId))
        linkage = ToLinkage(Expect(TokenType_Id).Str);
    auto name = Expect(TokenType_GlobalId).Str;
    std::vector<std::string> meta;
    while (At(TokenType_Meta)) meta.push_back(Skip().Str);
    Expect("=");
    const auto type = ParseType();
    const auto init = ParseConstant(type);
    return new GlobalVariable(type, std::move(name), linkage, init, std::move(meta));
}

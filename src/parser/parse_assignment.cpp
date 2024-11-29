#include <Brewer/Parser.hpp>
#include <Brewer/Value/GlobalValue.hpp>
#include <Brewer/Value/Value.hpp>

Brewer::Assignment* Brewer::Parser::ParseAssignment()
{
    const auto name = Expect(TokenType_LocalId).Str;
    Expect("=");
    const auto type = ParseType();
    const auto dst = m_Parent->Get(type, name);
    const auto src = ParseValue(type);
    return new Assignment(dst, src);
}

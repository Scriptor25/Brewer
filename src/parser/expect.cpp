#include <Brewer/Parser.hpp>

Brewer::Token Brewer::Parser::Expect(const TokenType type)
{
    if (!At(type))
        Error("unexpected token");
    return Skip();
}

bool Brewer::Parser::Expect(const std::string& value)
{
    if (!At(value))
        Error("unexpected token");
    NextToken();
    return true;
}

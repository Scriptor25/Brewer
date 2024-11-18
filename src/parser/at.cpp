#include <Brewer/Parser.hpp>

bool Brewer::Parser::AtEof() const
{
    return m_Token.Type == EofToken;
}

bool Brewer::Parser::At(const TokenType type) const
{
    return m_Token.Type == type;
}

bool Brewer::Parser::At(const std::string& value) const
{
    return m_Token.Value == value;
}

bool Brewer::Parser::NextAt(const TokenType type)
{
    if (!At(type)) return false;
    NextToken();
    return true;
}

bool Brewer::Parser::NextAt(const std::string& value)
{
    if (!At(value)) return false;
    NextToken();
    return true;
}

Brewer::Token Brewer::Parser::Skip()
{
    auto token = m_Token;
    NextToken();
    return token;
}

Brewer::Token Brewer::Parser::Expect(const TokenType type)
{
    if (!At(type)) Error("unexpected token");
    return Skip();
}

bool Brewer::Parser::Expect(const std::string& value)
{
    if (!At(value)) Error("unexpected token");
    NextToken();
    return true;
}

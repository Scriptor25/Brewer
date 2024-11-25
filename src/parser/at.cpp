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

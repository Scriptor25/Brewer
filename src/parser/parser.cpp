#include <Brewer/Parser.hpp>

Brewer::Parser::Parser(std::istream &stream, Module &dest)
    : m_Stream(stream),
      m_Dest(dest)
{
    Get();
    NextToken();
}

int Brewer::Parser::Get()
{
    return m_Tok = m_Stream.get();
}

bool Brewer::Parser::AtEof() const
{
    return m_Token.Type == TokenType_Eof;
}

bool Brewer::Parser::At(const TokenType type) const
{
    return m_Token.Type == type;
}

bool Brewer::Parser::At(const std::string &str) const
{
    return m_Token.Str == str;
}

bool Brewer::Parser::NextAt(const TokenType type)
{
    if (!At(type))
        return false;
    NextToken();
    return true;
}

bool Brewer::Parser::NextAt(const std::string &str)
{
    if (!At(str))
        return false;
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
    if (!At(type))
        Error("expected type {}, at {}", type, m_Token.Type);
    return Skip();
}

bool Brewer::Parser::Expect(const std::string &str)
{
    if (!At(str))
        Error("expected value '{}', at '{}'", str, m_Token.Str);
    NextToken();
    return true;
}

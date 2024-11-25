#include <Brewer/Parser.hpp>

Brewer::Token Brewer::Parser::Skip()
{
    auto token = m_Token;
    NextToken();
    return token;
}

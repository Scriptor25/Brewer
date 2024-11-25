#include <Brewer/Parser.hpp>

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

#include <Brewer/Parser.hpp>

Brewer::Parser::Parser(std::istream& stream, Module& dest)
    : m_Stream(stream), m_Dest(dest)
{
    Get();
    NextToken();
}

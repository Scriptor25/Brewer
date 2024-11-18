#include <Brewer/Parser.hpp>

int Brewer::Parser::Get()
{
    return m_Tok = m_Stream.get();
}

#include <Brewer/Parser.hpp>

void Brewer::Parser::Parse(std::istream& stream, Module& dest)
{
    Parser parser(stream, dest);
    while (!parser.At(EofToken))
        parser.ParseGlobal();
}

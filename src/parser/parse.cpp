#include <Brewer/Parser.hpp>

void Brewer::Parser::Parse(std::istream& stream, Module& dest)
{
    Parser parser(stream, dest);
    while (!parser.AtEof())
    {
        if (parser.At("global"))
        {
            parser.ParseGlobal();
            continue;
        }
        if (parser.At("define") || parser.At("declare"))
        {
            parser.ParseFunction();
            continue;
        }
        Error("unused token '{}'", parser.m_Token.Value);
    }
}

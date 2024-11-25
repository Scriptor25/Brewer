#include <Brewer/Module.hpp>
#include <Brewer/Parser.hpp>

void Brewer::Parser::Parse(std::istream& stream, Module& dest)
{
    Parser parser(stream, dest);
    while (!parser.AtEof())
    {
        const auto global = parser.ParseGlobalValue();
        dest.Append(global);
    }
}

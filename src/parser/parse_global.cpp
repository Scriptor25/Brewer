#include <Brewer/Parser.hpp>

void Brewer::Parser::ParseGlobal()
{
    if (At("define") || At("declare"))
        return ParseFunction();
}

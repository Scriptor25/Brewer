#include <Brewer/Parser.hpp>
#include <Brewer/Value/GlobalValue.hpp>

Brewer::GlobalValue *Brewer::Parser::ParseGlobalValue()
{
    if (At("var"))
        return ParseGlobalVariable();
    return ParseGlobalFunction();
}

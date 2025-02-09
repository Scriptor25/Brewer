#include <Brewer/Parser.hpp>
#include <Brewer/Value/Constant.hpp>

Brewer::Constant *Brewer::Parser::ParseConstant(Type *type)
{
    if (At(TokenType_Int))
        return ParseConstantInt(type);
    if (At(TokenType_Float))
        return ParseConstantFloat(type);
    if (At(TokenType_String) || At("["))
        return ParseConstantArray(type);

    Error("not a constant");
}

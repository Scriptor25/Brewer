#include <Brewer/Parser.hpp>
#include <Brewer/Type.hpp>
#include <Brewer/Value/Constant.hpp>

Brewer::ConstantInt* Brewer::Parser::ParseConstantInt(Type* type)
{
    const auto int_type = dynamic_cast<IntType*>(type);
    if (!int_type)
        Error("not an integer type");

    const auto val = Expect(TokenType_Int).Int;
    std::vector<std::string> meta;
    while (At(TokenType_Meta)) meta.push_back(Skip().Str);
    return new ConstantInt(int_type, val, std::move(meta));
}

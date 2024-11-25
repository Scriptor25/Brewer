#include <Brewer/Parser.hpp>
#include <Brewer/Type.hpp>
#include <Brewer/Value/Constant.hpp>

Brewer::ConstantFloat* Brewer::Parser::ParseConstantFloat(Type* type)
{
    const auto float_type = dynamic_cast<FloatType*>(type);
    if (!float_type)
        Error("not a float type");

    const auto val = Expect(TokenType_Float).Flt;
    return new ConstantFloat(float_type, val);
}

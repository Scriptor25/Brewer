#include <Brewer/Parser.hpp>
#include <Brewer/Value/Value.hpp>

Brewer::Instruction* Brewer::Parser::ParseInstruction(Type* type, const std::string& code_str)
{
    const auto code = ToCode(code_str);
    if (!code) Error("not a code: '{}'", code_str);
    std::vector<std::string> meta;
    while (At(TokenType_Meta)) meta.push_back(Skip().Str);
    std::vector<Value*> operands;
    do operands.push_back(ParseOperand());
    while (NextAt(","));
    return new Instruction(type, code, operands, std::move(meta));
}

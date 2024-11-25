#include <Brewer/Parser.hpp>
#include <Brewer/Value/Value.hpp>

Brewer::Instruction* Brewer::Parser::ParseInstruction(Type* type, const std::string& code_str)
{
    const auto code = ToCode(code_str);
    if (!code) Error("not a code");
    std::vector<Value*> operands;
    do operands.push_back(ParseOperand());
    while (NextAt(","));
    return new Instruction(type, code, operands);
}

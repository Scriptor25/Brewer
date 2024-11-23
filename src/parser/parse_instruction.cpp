#include <map>
#include <vector>
#include <Brewer/Parser.hpp>
#include <Brewer/Value/Instruction.hpp>

Brewer::Instruction* Brewer::Parser::ParseInstruction(Type* type, const std::string& name, const std::string& code_name)
{
    const auto code = ToCode(code_name);
    std::vector<Value*> operands;
    do operands.push_back(ParseOperand());
    while (NextAt(","));

    return new Instruction(type, name, code, operands);
}

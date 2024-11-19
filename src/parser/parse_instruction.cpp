#include <map>
#include <vector>
#include <Brewer/Parser.hpp>
#include <Brewer/Value/Instruction.hpp>

Brewer::Instruction* Brewer::Parser::ParseInstruction(Type* type, const std::string& name)
{
    const auto code = ToCode(name);
    std::vector<Value*> operands;
    do operands.push_back(ParseOperand());
    while (NextAt(","));

    return new Instruction(type, {}, code, operands);
}

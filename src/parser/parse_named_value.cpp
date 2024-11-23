#include <Brewer/Context.hpp>
#include <Brewer/Module.hpp>
#include <Brewer/Parser.hpp>
#include <Brewer/Value/Block.hpp>
#include <Brewer/Value/Instruction.hpp>

Brewer::NamedValue* Brewer::Parser::ParseNamedValue()
{
    std::string name;
    Type* type{};
    if (At(LocalToken))
    {
        name = Skip().Value;
        Expect("=");
        type = ParseType();
    }

    const auto code_name = Expect(IdToken).Value;

    if (name.empty() && !type && NextAt(":"))
        return new Block(m_Dest.GetContext().GetBlockType(), code_name);

    return ParseInstruction(type, name, code_name);
}

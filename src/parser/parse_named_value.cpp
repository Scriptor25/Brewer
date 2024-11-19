#include <Brewer/Context.hpp>
#include <Brewer/Module.hpp>
#include <Brewer/Parser.hpp>
#include <Brewer/Value/Block.hpp>
#include <Brewer/Value/Instruction.hpp>

Brewer::NamedValue* Brewer::Parser::ParseNamedValue()
{
    if (At(LocalToken))
    {
        const auto name = Skip().Value;
        Expect("=");
        const auto type = ParseType();
        const auto opname = Expect(IdToken).Value;
        const auto value = ParseInstruction(type, opname);
        value->SetName(name);
        return value;
    }

    const auto opname = Expect(IdToken).Value;
    if (NextAt(":"))
        return new Block(m_Dest.GetContext().GetBlockType(), opname);
    return ParseInstruction({}, opname);
}

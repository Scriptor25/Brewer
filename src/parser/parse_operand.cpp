#include <Brewer/Context.hpp>
#include <Brewer/Module.hpp>
#include <Brewer/Parser.hpp>
#include <Brewer/Type.hpp>
#include <Brewer/Value/Constant.hpp>
#include <Brewer/Value/GlobalValue.hpp>

Brewer::Value* Brewer::Parser::ParseOperand()
{
    if (At(TokenType_LocalId))
    {
        const auto type = m_Dest.GetContext().GetBlockType();
        const auto name = Skip().Str;
        return m_Parent->Get(type, name);
    }

    const auto type = ParseType();

    if (At(TokenType_LocalId))
    {
        const auto name = Skip().Str;
        return m_Parent->Get(type, name);
    }

    if (At(TokenType_GlobalId))
    {
        const auto name = Skip().Str;
        return m_Dest.Get(dynamic_cast<PointerType*>(type), name);
    }

    return ParseConstant(type);
}

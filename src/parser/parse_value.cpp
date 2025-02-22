#include <Brewer/Context.hpp>
#include <Brewer/Module.hpp>
#include <Brewer/Parser.hpp>
#include <Brewer/Value/Constant.hpp>
#include <Brewer/Value/GlobalValue.hpp>
#include <Brewer/Value/NamedValue.hpp>

Brewer::Value *Brewer::Parser::ParseValue(Type *type)
{
    if (At(TokenType_Id))
    {
        const auto name = Skip().Str;
        if (!type && NextAt(":"))
        {
            std::vector<std::string> meta;
            while (At(TokenType_Meta))
                meta.push_back(Skip().Str);
            return new FunctionBlock(m_Dest.GetContext().GetBlockType(), name, std::move(meta));
        }
        return ParseInstruction(type, name);
    }
    if (!At(TokenType_LocalId))
        return ParseConstant(type);
    if (type)
        return m_Parent->Get(type, Skip().Str);
    return ParseAssignment();
}

#include <Brewer/Context.hpp>
#include <Brewer/Module.hpp>
#include <Brewer/Parser.hpp>
#include <Brewer/Type.hpp>
#include <Brewer/Value/Constant.hpp>
#include <Brewer/Value/Function.hpp>
#include <Brewer/Value/GlobalValue.hpp>

Brewer::Value* Brewer::Parser::ParseOperand()
{
    if (At(LocalToken))
        return m_Parent->GetValue(m_Dest.GetContext().GetBlockType(), Skip().Value);

    const auto type = ParseType();

    if (At(LocalToken))
        return m_Parent->GetValue(type, Skip().Value);

    if (At(GlobalToken))
        return m_Dest.GetGlobalValue(type, Skip().Value);

    return ParseConstant(type);
}

#include <Brewer/Context.hpp>
#include <Brewer/Module.hpp>
#include <Brewer/Parser.hpp>

void Brewer::Parser::ParseGlobalType()
{
    Expect("type");
    const auto name = Expect(TokenType_Id).Str;
    Expect("=");
    const auto type = ParseType();
    m_Dest.GetContext().GlobalType(name, type);
}

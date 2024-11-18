#pragma once

#include <istream>
#include <Brewer/Brewer.hpp>

namespace Brewer
{
    enum TokenType
    {
        EofToken,
    };

    struct Token
    {
        TokenType Type = EofToken;
        std::string Value;
    };

    class Parser
    {
    public:
        static void Parse(std::istream& stream, Module& dest);

    private:
        explicit Parser(std::istream& stream, Module& dest);

        int Get();
        Token& NextToken();

        bool At(TokenType type) const;
        bool At(const std::string& value) const;

        void ParseGlobal();

        std::istream& m_Stream;
        Module& m_Dest;

        int m_Tok;
        Token m_Token;
    };
}

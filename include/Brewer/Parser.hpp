#pragma once

#include <cstdint>
#include <Brewer/Brewer.hpp>

namespace Brewer
{
    enum TokenType
    {
        EofToken,
        IdToken,
        GlobalToken,
        LocalToken,
        IntToken,
        FloatToken,
    };

    struct Token
    {
        TokenType Type = EofToken;
        std::string Value;
        uint64_t IntValue;
        double FloatValue;
    };

    class Parser
    {
    public:
        static void Parse(std::istream& stream, Module& dest);

    private:
        explicit Parser(std::istream& stream, Module& dest);

        int Get();
        Token& NextToken();

        [[nodiscard]] bool AtEof() const;
        [[nodiscard]] bool At(TokenType type) const;
        [[nodiscard]] bool At(const std::string& value) const;

        bool NextAt(TokenType type);
        bool NextAt(const std::string& value);

        Token Skip();

        Token Expect(TokenType type);
        bool Expect(const std::string& value);

        Type* ParseType();

        void ParseGlobal();
        void ParseFunction();

        std::istream& m_Stream;
        Module& m_Dest;

        int m_Tok;
        Token m_Token;
    };
}

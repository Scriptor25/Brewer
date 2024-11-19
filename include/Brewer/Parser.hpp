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
        StringToken,
        OtherToken,
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

        GlobalVariable* ParseGlobal();
        Function* ParseFunction();

        NamedValue* ParseNamedValue();
        Instruction* ParseInstruction(Type* type, const std::string& name);
        Value* ParseOperand();
        Constant* ParseConstant(Type* type);

        std::istream& m_Stream;
        Module& m_Dest;
        Function* m_Parent{};

        int m_Tok;
        Token m_Token;
    };
}

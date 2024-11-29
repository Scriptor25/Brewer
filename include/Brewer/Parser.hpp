#pragma once

#include <cstdint>
#include <Brewer/Brewer.hpp>

namespace Brewer
{
    enum TokenType
    {
        TokenType_Eof,
        TokenType_Id,
        TokenType_GlobalId,
        TokenType_LocalId,
        TokenType_Meta,
        TokenType_Int,
        TokenType_Float,
        TokenType_String,
        TokenType_Other,
    };

    struct Token
    {
        TokenType Type = TokenType_Eof;
        std::string Str;
        uint64_t Int;
        double Flt;
    };

    class Parser
    {
    public:
        static void Parse(std::istream& stream, Module& dest);

    private:
        Parser(std::istream& stream, Module& dest);

        int Get();
        Token& NextToken();

        [[nodiscard]] bool AtEof() const;
        [[nodiscard]] bool At(TokenType type) const;
        [[nodiscard]] bool At(const std::string& str) const;

        bool NextAt(TokenType type);
        bool NextAt(const std::string& str);

        Token Skip();

        Token Expect(TokenType type);
        bool Expect(const std::string& str);

        Type* ParseType();
        void ParseGlobalType();

        Value* ParseValue(Type* type);
        Instruction* ParseInstruction(Type* type, const std::string& code_str);

        Assignment* ParseAssignment();

        Constant* ParseConstant(Type* type);
        ConstantInt* ParseConstantInt(Type* type);
        ConstantFloat* ParseConstantFloat(Type* type);
        ConstantArray* ParseConstantArray(Type* type);

        GlobalValue* ParseGlobalValue();
        GlobalVariable* ParseGlobalVariable();
        GlobalFunction* ParseGlobalFunction();

        Value* ParseOperand();

        std::istream& m_Stream;
        Module& m_Dest;
        GlobalFunction* m_Parent{};

        int m_Tok;
        Token m_Token;
    };
}

namespace std
{
    template <>
    struct formatter<Brewer::TokenType> : formatter<string>
    {
        template <typename FormatContext>
        auto format(Brewer::TokenType type, FormatContext& ctx) const
        {
            std::string str;
            switch (type)
            {
            case Brewer::TokenType_Eof:
                str = "Eof";
                break;
            case Brewer::TokenType_Id:
                str = "Id";
                break;
            case Brewer::TokenType_GlobalId:
                str = "GlobalId";
                break;
            case Brewer::TokenType_LocalId:
                str = "LocalId";
                break;
            case Brewer::TokenType_Int:
                str = "Int";
                break;
            case Brewer::TokenType_Float:
                str = "Float";
                break;
            case Brewer::TokenType_String:
                str = "String";
                break;
            case Brewer::TokenType_Other:
                str = "Other";
                break;
            }
            return formatter<string>::format(str, ctx);
        }
    };
}

#include <iostream>
#include <Brewer/Parser.hpp>

static bool is_digit(const int c, const int base)
{
    switch (base)
    {
    case 2: return c == '0' || c == '1';
    case 8: return '0' <= c && c <= '7';
    case 10: return isdigit(c);
    case 16: return isxdigit(c);
    default: return false;
    }
}

static bool is_id(const int c)
{
    return isalnum(c) || c == '.' || c == '$' || c == '_';
}

static uint64_t to_int(const std::string& value, const int base)
{
    return std::stoull(value, nullptr, base);
}

Brewer::Token& Brewer::Parser::NextToken()
{
    enum State
    {
        NoState,
        CommentState,
        IdState,
        IntState,
        FloatState,
    };

    auto state = NoState;
    int base = 0;
    std::string value;

    while (m_Tok >= 0)
    {
        switch (state)
        {
        case NoState:
            switch (m_Tok)
            {
            case ';':
                state = CommentState;
                break;

            case '0':
                Get();
                switch (m_Tok)
                {
                case 'b':
                case 'B':
                    state = IntState;
                    base = 2;
                    Get();
                    break;

                case 'x':
                case 'X':
                    state = IntState;
                    base = 16;
                    Get();
                    break;

                case '.':
                    state = FloatState;
                    value = "0.";
                    Get();
                    break;

                default:
                    state = IntState;
                    value = "0";
                    base = 8;
                    break;
                }
                break;

            default:
                if (is_digit(m_Tok, 10))
                {
                    state = IntState;
                    base = 10;
                    break;
                }
                if (is_id(m_Tok))
                {
                    state = IdState;
                    break;
                }
                Get();
                break;
            }
            break;

        case CommentState:
            if (m_Tok == '\n')
                state = NoState;
            Get();
            break;

        case IdState:
            if (!is_id(m_Tok))
                return m_Token = {IdToken, value};
            value += static_cast<char>(m_Tok);
            Get();
            break;

        case IntState:
            if (base == 10 && m_Tok == '.')
            {
                state = FloatState;
                value += static_cast<char>(m_Tok);
                Get();
                break;
            }
            if (!is_digit(m_Tok, base))
                return m_Token = {IntToken, value, to_int(value, base)};
            value += static_cast<char>(m_Tok);
            Get();
            break;

        case FloatState:
            if (m_Tok == 'e' || m_Tok == 'E')
            {
                value += static_cast<char>(m_Tok);
                Get();
                if (m_Tok == '+' || m_Tok == '-')
                {
                    value += static_cast<char>(m_Tok);
                    Get();
                }
            }
            if (!is_digit(m_Tok, 10))
                return m_Token = {FloatToken, value, 0, std::stod(value)};
            value += static_cast<char>(m_Tok);
            Get();
            break;
        }
    }

    return m_Token = {};
}

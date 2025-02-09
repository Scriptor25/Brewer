#include <iostream>
#include <Brewer/Parser.hpp>

static bool is_digit(const int c, const int base)
{
    switch (base)
    {
        case 2:
            return c == '0' || c == '1';
        case 8:
            return '0' <= c && c <= '7';
        case 10:
            return isdigit(c);
        case 16:
            return isxdigit(c);
        default:
            return false;
    }
}

static bool is_id(const int c)
{
    return isalnum(c) || c == '.' || c == '$' || c == '_';
}

static uint64_t to_int(const std::string &value, const int base)
{
    return std::stoull(value, nullptr, base);
}

Brewer::Token &Brewer::Parser::NextToken()
{
    enum State
    {
        State_None,
        State_Comment,
        State_Id,
        State_Int,
        State_Float,
        State_String,
    };

    auto state = State_None;
    auto base = 0;
    std::string value;
    auto type = TokenType_Eof;

    while (m_Tok >= 0)
    {
        switch (state)
        {
            case State_None:
                switch (m_Tok)
                {
                    case ';':
                        state = State_Comment;
                        break;

                    case '@':
                        state = State_Id;
                        type = TokenType_GlobalId;
                        Get();
                        break;

                    case '%':
                        state = State_Id;
                        type = TokenType_LocalId;
                        Get();
                        break;

                    case '!':
                        state = State_Id;
                        type = TokenType_Meta;
                        Get();
                        break;

                    case '0':
                        Get();
                        switch (m_Tok)
                        {
                            case 'b':
                            case 'B':
                                state = State_Int;
                                base = 2;
                                Get();
                                break;

                            case 'x':
                            case 'X':
                                state = State_Int;
                                base = 16;
                                Get();
                                break;

                            case '.':
                                state = State_Float;
                                value = "0.";
                                Get();
                                break;

                            default:
                                state = State_Int;
                                value = "0";
                                base = 8;
                                break;
                        }
                        break;

                    case '"':
                        state = State_String;
                        Get();
                        break;

                    case '(':
                    case ')':
                    case '{':
                    case '}':
                    case '[':
                    case ']':
                    case ',':
                    case '.':
                    case ':':
                    case '=':
                        value += static_cast<char>(m_Tok);
                        Get();
                        return m_Token = {TokenType_Other, std::move(value)};

                    default:
                        if (is_digit(m_Tok, 10))
                        {
                            state = State_Int;
                            base = 10;
                            break;
                        }
                        if (is_id(m_Tok))
                        {
                            state = State_Id;
                            type = TokenType_Id;
                            break;
                        }
                        Get();
                        break;
                }
                break;

            case State_Comment:
                if (m_Tok == '\n')
                    state = State_None;
                Get();
                break;

            case State_Id:
                if (!is_id(m_Tok))
                    return m_Token = {type, std::move(value)};
                value += static_cast<char>(m_Tok);
                Get();
                break;

            case State_Int:
                if (base == 10 && m_Tok == '.')
                {
                    state = State_Float;
                    value += static_cast<char>(m_Tok);
                    Get();
                    break;
                }
                if (!is_digit(m_Tok, base))
                {
                    const auto int_value = to_int(value, base);
                    return m_Token = {TokenType_Int, std::move(value), int_value};
                }
                value += static_cast<char>(m_Tok);
                Get();
                break;

            case State_Float:
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
                {
                    const auto float_value = std::stod(value);
                    return m_Token = {TokenType_Float, std::move(value), 0, float_value};
                }
                value += static_cast<char>(m_Tok);
                Get();
                break;

            case State_String:
                if (m_Tok == '"')
                {
                    Get();
                    return m_Token = {TokenType_String, std::move(value)};
                }
                if (m_Tok == '\\')
                {
                    std::string val;
                    val += static_cast<char>(Get());
                    val += static_cast<char>(Get());
                    m_Tok = std::stoi(val, nullptr, 16);
                }
                value += static_cast<char>(m_Tok);
                Get();
                break;
        }
    }

    return m_Token = {};
}

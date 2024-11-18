#include <iostream>
#include <Brewer/Parser.hpp>

Brewer::Token& Brewer::Parser::NextToken()
{
    enum State
    {
        NoState,
        CommentState,
    };

    auto state = NoState;

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

            default:
                std::cout << static_cast<char>(m_Tok);
                Get();
                break;
            }
            break;

        case CommentState:
            if (m_Tok == '\n')
                state = NoState;
            Get();
            break;
        }
    }

    return m_Token = {};
}

#pragma once

#include <Brewer/Value/Value.hpp>

namespace Brewer
{
    class Argument : public Value
    {
    public:
        Argument(Type* type, std::string name, unsigned index);

    private:
        unsigned m_Index;
    };
}

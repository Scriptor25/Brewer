#pragma once

#include <Brewer/Value/NamedValue.hpp>

namespace Brewer
{
    class Argument : public NamedValue
    {
    public:
        Argument(Type* type, std::string name, unsigned index);

    private:
        unsigned m_Index;
    };
}

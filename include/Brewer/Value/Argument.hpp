#pragma once

#include <Brewer/Value/NamedValue.hpp>

namespace Brewer
{
    class Argument : public NamedValue
    {
    public:
        Argument(Type* type, std::string name, unsigned index);

        [[nodiscard]] unsigned GetIndex() const;

    private:
        unsigned m_Index;
    };
}

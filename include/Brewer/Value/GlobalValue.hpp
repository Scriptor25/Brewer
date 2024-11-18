#pragma once

#include <Brewer/Value/Value.hpp>

namespace Brewer
{
    class GlobalValue : public Value
    {
    public:
        enum LinkageType
        {
        };

        GlobalValue(Type* type, std::string name, LinkageType linkage);
    };
}

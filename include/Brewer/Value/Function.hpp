#pragma once

#include <Brewer/Value/GlobalValue.hpp>

namespace Brewer
{
    class Function : public GlobalValue
    {
    public:
        Function(FunctionType* function_type, std::string name, LinkageType linkage);
    };
}

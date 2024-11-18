#pragma once

#include <Brewer/Brewer.hpp>

namespace Brewer
{
    class Builder
    {
    public:
        explicit Builder(Context& context);

    private:
        Context& m_Context;
    };
}

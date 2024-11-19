#pragma once

#include <Brewer/Value/GlobalValue.hpp>

namespace Brewer
{
    class GlobalVariable : public GlobalValue
    {
    public:
        GlobalVariable(Type* type, std::string name, LinkageType linkage, Constant* initializer);

        std::ostream& Print(std::ostream& os) const override;

    private:
        Constant* m_Initializer;
    };
}

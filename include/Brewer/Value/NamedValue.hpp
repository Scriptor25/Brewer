#pragma once

#include <Brewer/Value/Value.hpp>

namespace Brewer
{
    class NamedValue : public Value
    {
    public:
        NamedValue(Type* type, std::string name);

        std::ostream& PrintIR(std::ostream& os) const override;
        std::ostream& PrintIROperand(std::ostream& os) const override;

        [[nodiscard]] const std::string& GetName() const;

    private:
        std::string m_Name;
    };
}

#pragma once

#include <Brewer/Value/Value.hpp>

namespace Brewer
{
    class NamedValue : public Value
    {
    public:
        NamedValue(Type* type, std::string name);

        std::ostream& Print(std::ostream& os) const override;
        std::ostream& PrintOperand(std::ostream& os) const override;

        [[nodiscard]] const std::string& GetName() const;
        void SetName(const std::string& name);

    private:
        std::string m_Name;
    };
}

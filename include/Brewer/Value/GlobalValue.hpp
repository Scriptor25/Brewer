#pragma once

#include <Brewer/Value/NamedValue.hpp>

namespace Brewer
{
    class GlobalValue : public NamedValue
    {
    public:
        enum LinkageType
        {
            /**
             * not visible outside its declaring block/function
             */
            NoLinkage,
            /**
             * resolved by linker across all modules
             */
            ExternLinkage,
            /**
             * local to its declaring module
             */
            InternLinkage,
            /**
             * overridden by strong symbols, if present
             */
            WeakLinkage,
            /**
             * resolved to a single allocation
             */
            CommonLinkage,
            /**
             * resolved to a single definition
             */
            TentativeLinkage,
        };

        GlobalValue(Type* element_type, std::string name, LinkageType linkage);

        std::ostream& PrintIR(std::ostream& os) const override;
        std::ostream& PrintIROperand(std::ostream& os) const override;

        [[nodiscard]] LinkageType GetLinkage() const;

    private:
        LinkageType m_Linkage;
    };

    GlobalValue::LinkageType ToLinkage(const std::string& name);
    std::string ToString(GlobalValue::LinkageType linkage);

    std::ostream& operator<<(std::ostream& os, GlobalValue::LinkageType linkage);
}

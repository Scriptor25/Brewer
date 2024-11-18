#pragma once

#include <Brewer/Value/Value.hpp>

namespace Brewer
{
    class GlobalValue : public Value
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
            ExternalLinkage,
            /**
             * local to its declaring module
             */
            InternalLinkage,
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
    };
}

#pragma once

#include <Brewer/Brewer.hpp>

namespace Brewer
{
    class Value
    {
    public:
        Value(Type* type, std::string name);
        virtual ~Value() = default;

        [[nodiscard]] Value* GetPrevious() const;
        [[nodiscard]] Value* GetNext() const;

        void SetPrevious(Value* value);
        void SetNext(Value* value);

    private:
        Value* m_Previous{};
        Value* m_Next{};

        Type* m_Type;
        std::string m_Name;
    };
}

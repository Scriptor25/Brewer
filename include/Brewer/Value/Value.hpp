#pragma once

#include <vector>
#include <Brewer/Brewer.hpp>

namespace Brewer
{
    class Value
    {
    public:
        explicit Value(Type* type);
        virtual ~Value() = default;

        [[nodiscard]] Type* GetType() const;

        virtual std::ostream& Print(std::ostream& os) const = 0;
        virtual std::ostream& PrintOperand(std::ostream& os) const = 0;

        std::ostream& PrintUseList(std::ostream& os) const;

        void AddUse(Value* use);
        void ReplaceAllUses(Value* new_value);

        virtual void ReplaceUseOf(Value* old_value, Value* new_value) = 0;

    private:
        Type* m_Type;

        std::vector<Value*> m_UseList;

        Value* m_Previous{};
        Value* m_Next{};
    };
}

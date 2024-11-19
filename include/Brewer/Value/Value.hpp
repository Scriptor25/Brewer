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

        std::ostream& PrintUseList(std::ostream& os) const;

        virtual std::ostream& Print(std::ostream& os) const = 0;
        virtual std::ostream& PrintOperand(std::ostream& os) const = 0;

        void AddUse(Value* use);
        void ReplaceAllUses(Value* new_value);

        virtual void ReplaceUseOf(Value* old_value, Value* new_value);

        /**
         * Inserts this directly before the target value
         * @param value the target
         */
        void InsertBefore(Value* value);
        /**
         * Inserts this directly after the target value
         * @param value the target
         */
        void InsertAfter(Value* value);
        /**
         * Prepends this to the list of values containing the target value
         * @param value the target
         */
        void PrependBefore(Value* value);
        /**
         * Appends this to the list of values containing the target value
         * @param value the target
         */
        void AppendAfter(Value* value);

    private:
        Type* m_Type;

        std::vector<Value*> m_UseList;

        Value* m_Previous{};
        Value* m_Next{};
    };
}

#pragma once

#include <format>
#include <sstream>
#include <string>
#include <vector>
#include <Brewer/Brewer.hpp>

namespace Brewer
{
    class Value
    {
    public:
        explicit Value(Type* type);
        virtual ~Value();

        unsigned GetIndex() const;

        template <typename T = Type>
        [[nodiscard]] T* GetType() const
        {
            return dynamic_cast<T*>(m_Type);
        }

        std::ostream& PrintUseList(std::ostream& os) const;

        void Print(Printer* printer);
        virtual std::ostream& PrintIR(std::ostream& os) const = 0;
        virtual std::ostream& PrintIROperand(std::ostream& os) const = 0;

        void AddUse(Value* use);
        void ReplaceAllUses(Value* new_value);

        virtual void ReplaceUseOf(Value* old_value, Value* new_value);

    private:
        unsigned m_Index;

        Type* m_Type;
        std::vector<Value*> m_UseList;
    };
}

template <typename T>
concept DerivedValue = std::is_base_of_v<Brewer::Value, T>;

template <DerivedValue T>
struct std::formatter<T*> : std::formatter<std::string>
{
    template <typename FormatContext>
    auto format(T* type, FormatContext& ctx) const
    {
        std::stringstream os;
        type->PrintIROperand(os);
        return std::formatter<std::string>::format(os.str(), ctx);
    }
};

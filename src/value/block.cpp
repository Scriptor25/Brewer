#include <utility>
#include <Brewer/Type.hpp>
#include <Brewer/Value/Block.hpp>

Brewer::Block::Block(BlockType* type, std::string name)
    : NamedValue(type, std::move(name))
{
}

std::ostream& Brewer::Block::Print(std::ostream& os) const
{
    if (!GetName().empty())
        os << GetName() << ':' << std::endl;
    for (const auto& value : m_Values)
        value->Print(os << "  ") << std::endl;
    return os;
}

std::ostream& Brewer::Block::PrintOperand(std::ostream& os) const
{
    if (!GetType() || GetName().empty())
        return os << "<null>";
    return os << '%' << GetName();
}

void Brewer::Block::ReplaceUseOf(Value* old_value, Value* new_value)
{
    for (auto& value : m_Values)
        if (value == old_value)
        {
            value = new_value;
            break;
        }
}

Brewer::NamedValue* Brewer::Block::GetValue(const Type* type, const std::string& name)
{
    for (const auto& value : m_Values)
        if (const auto named = dynamic_cast<NamedValue*>(value); named && named->GetName() == name)
        {
            if (named->GetType() != type) Error("types do not match");
            return named;
        }
    return {};
}

void Brewer::Block::Append(Value* value)
{
    value->AddUse(this);
    m_Values.push_back(value);
}

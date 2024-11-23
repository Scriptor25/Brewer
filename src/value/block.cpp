#include <utility>
#include <Brewer/Type.hpp>
#include <Brewer/Value/Block.hpp>

Brewer::Block::Block(BlockType* type, std::string name)
    : NamedValue(type, std::move(name))
{
}

std::ostream& Brewer::Block::PrintIR(std::ostream& os) const
{
    if (!GetName().empty())
        os << GetName() << ':' << std::endl;
    for (const auto& value : m_Values)
        value->PrintIR(os << "  ") << std::endl;
    return os;
}

std::ostream& Brewer::Block::PrintIROperand(std::ostream& os) const
{
    if (!GetType() || GetName().empty())
        return os << "<null>";
    return os << '%' << GetName();
}

Brewer::NamedValue* Brewer::Block::GetValue(Type* type, const std::string& name) const
{
    for (const auto& value : m_Values)
        if (const auto named = dynamic_cast<NamedValue*>(value); named && named->GetName() == name)
        {
            if (named->GetType() != type)
                Error("type mismatch: {} != {}", named->GetType(), type);
            return named;
        }
    return {};
}

void Brewer::Block::Append(Value* value)
{
    value->AddUse(this);
    m_Values.push_back(value);
}

Brewer::Value* Brewer::Block::GetValue(const unsigned i) const
{
    return m_Values[i];
}

unsigned Brewer::Block::GetNumValues() const
{
    return m_Values.size();
}

#include <algorithm>
#include <Brewer/Value/Value.hpp>

static unsigned INDEX = 0;

Brewer::Value::Value(Type* type, std::vector<std::string>&& meta)
    : m_Index(INDEX++), m_Type(type), m_Meta(std::move(meta))
{
}

unsigned Brewer::Value::GetIndex() const
{
    return m_Index;
}

Brewer::Type* Brewer::Value::GetType() const
{
    return m_Type;
}

bool Brewer::Value::GetMeta(const std::string& name) const
{
    return std::ranges::any_of(m_Meta, [&](const std::string& meta) { return meta == name; });
}

uint64_t Brewer::Value::GetNumUses() const
{
    return m_UseList.size();
}

void Brewer::Value::AddUse(Value* user)
{
    m_UseList.push_back(user);
}

void Brewer::Value::RemoveUse(Value* user)
{
    for (auto it = m_UseList.begin(); it != m_UseList.end(); ++it)
        if (*it == user)
        {
            m_UseList.erase(it);
            break;
        }
}

void Brewer::Value::ReplaceWith(Value* new_value)
{
    for (const auto& use : m_UseList)
    {
        use->Replace(this, new_value);
        new_value->AddUse(use);
    }
    m_UseList.clear();
    delete this;
}

std::ostream& Brewer::Value::PrintUseList(std::ostream& os) const
{
    os << "; ";
    if (m_UseList.empty())
        return os << "no uses";
    return os << m_UseList.size();
}

Brewer::Value::~Value()
{
    --INDEX;
}

bool Brewer::Value::NotNull() const
{
    return true;
}

bool Brewer::Value::IsTerminator() const
{
    return false;
}

bool Brewer::Value::RequiresDestination() const
{
    return false;
}

uint64_t Brewer::Value::GetNumAllocaBytes() const
{
    return 0;
}

void Brewer::Value::Replace(Value* old_value, Value* new_value)
{
}

std::ostream& Brewer::Value::PrintIR(std::ostream& os) const
{
    return PrintOperandIR(os, !GetType());
}

#include <Brewer/Value/Value.hpp>

static unsigned INDEX = 0;

Brewer::Value::Value(Type* type)
    : m_Index(INDEX++), m_Type(type)
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

bool Brewer::Value::NeedsDestination() const
{
    return false;
}

uint64_t Brewer::Value::CountAlloca() const
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

#include <iostream>
#include <Brewer/Printer/Printer.hpp>
#include <Brewer/Value/Value.hpp>

static unsigned INDEX = 0;

Brewer::Value::Value(Type* type)
    : m_Index(INDEX++), m_Type(type)
{
}

Brewer::Value::~Value()
{
    --INDEX;
}

unsigned Brewer::Value::GetIndex() const
{
    return m_Index;
}

std::ostream& Brewer::Value::PrintUseList(std::ostream& os) const
{
    if (m_UseList.empty())
        return os << "; no uses";
    os << "; ";
    for (unsigned i = 0; i < m_UseList.size(); ++i)
    {
        if (i > 0) os << ", ";
        m_UseList[i]->PrintIROperand(os);
    }
    return os;
}

void Brewer::Value::Print(Printer* printer)
{
    printer->Print(this);
}

void Brewer::Value::AddUse(Value* use)
{
    m_UseList.push_back(use);
}

void Brewer::Value::ReplaceAllUses(Value* new_value)
{
    for (const auto& use : m_UseList)
    {
        use->ReplaceUseOf(this, new_value);
        new_value->AddUse(use);
    }
    delete this;
}

void Brewer::Value::ReplaceUseOf(Value* old_value, Value* new_value)
{
    Error("this does not use anything");
}

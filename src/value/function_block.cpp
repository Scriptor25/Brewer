#include <Brewer/Type.hpp>
#include <Brewer/Value/NamedValue.hpp>

Brewer::FunctionBlock::FunctionBlock(BlockType* type, std::string name)
    : NamedValue(type, std::move(name))
{
}

Brewer::Value* Brewer::FunctionBlock::GetValue(unsigned i) const
{
    return {};
}

unsigned Brewer::FunctionBlock::GetNumValues() const
{
    return 0;
}

void Brewer::FunctionBlock::Append(Value* value)
{
}

Brewer::NamedValue* Brewer::FunctionBlock::Get(Type* type, const std::string& name) const
{
    return {};
}

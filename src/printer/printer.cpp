#include <Brewer/Printer/Printer.hpp>

Brewer::Printer::Printer(std::ostream& stream)
    : m_Stream(stream)
{
}

std::ostream& Brewer::Printer::S() const
{
    return m_Stream;
}

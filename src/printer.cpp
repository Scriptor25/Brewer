#include <Brewer/Printer.hpp>

Brewer::ASMPrinterBase::ASMPrinterBase(std::ostream& stream)
    : m_Stream(stream)
{
}

std::ostream& Brewer::ASMPrinterBase::S() const
{
    return m_Stream;
}

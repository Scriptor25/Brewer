#include <iostream>
#include <Brewer/Brewer.hpp>

void Brewer::Error(const std::string& message)
{
    std::cerr << message << std::endl;
    throw std::runtime_error(message);
}

#include <fstream>
#include <Brewer/Brewer.hpp>
#include <Brewer/Parser.hpp>

int main()
{
    const std::string filename = "example/example.b";
    std::ifstream stream(filename);
    Brewer::Parser parser(stream, filename);
    stream.close();
}

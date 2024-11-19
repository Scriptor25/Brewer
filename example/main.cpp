#include <fstream>
#include <iostream>
#include <Brewer/Brewer.hpp>
#include <Brewer/Context.hpp>
#include <Brewer/Module.hpp>
#include <Brewer/Parser.hpp>

int main()
{
    std::ifstream stream("example/example.b");
    Brewer::Context context;
    Brewer::Module module(context);
    Brewer::Parser::Parse(stream, module);
    stream.close();

    module.Print(std::cerr);
}

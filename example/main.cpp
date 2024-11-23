#include <fstream>
#include <iostream>
#include <Brewer/Brewer.hpp>
#include <Brewer/Context.hpp>
#include <Brewer/Module.hpp>
#include <Brewer/Parser.hpp>
#include <Brewer/Printer/X86Printer.hpp>

int main()
{
    Brewer::Context context;
    Brewer::Module module(context);

    std::ifstream is("example/example.b");
    Brewer::Parser::Parse(is, module);
    is.close();

    std::ofstream os("example/example.s");
    Brewer::X86Printer printer(os);
    module.Print(&printer);
    os.close();
}

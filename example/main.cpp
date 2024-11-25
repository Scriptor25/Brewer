#include <fstream>
#include <iostream>
#include <Brewer/Brewer.hpp>
#include <Brewer/Context.hpp>
#include <Brewer/Module.hpp>
#include <Brewer/Parser.hpp>
#include <Brewer/Printer/X86Printer.hpp>

int main()
{
    constexpr auto input = "example/example.b";
    constexpr auto output = "example/example.s";

    Brewer::Context context;
    Brewer::Module module(context, input);

    std::ifstream is(input);
    Brewer::Parser::Parse(is, module);
    is.close();

    std::ofstream os(output);
    Brewer::X86Printer printer(os);
    module.Print(&printer);
    os.close();
}

#include <fstream>
#include <iostream>
#include <Brewer/Brewer.hpp>
#include <Brewer/Context.hpp>
#include <Brewer/Module.hpp>
#include <Brewer/Parser.hpp>
#include <Brewer/Platform/X86/ASMPrinter.hpp>

int main(const int argc, const char* const* argv)
{
    const auto input = argv[1];
    const auto output = argv[2];

    Brewer::Context context;
    Brewer::Module module(context, input);

    std::ifstream is(input);
    Brewer::Parser::Parse(is, module);
    is.close();

    module.ValidateAndOptimize();

    std::ofstream os(output);
    Brewer::Platform::X86::ASMPrinter printer(os);
    module.Print(&printer);
    os.close();
}

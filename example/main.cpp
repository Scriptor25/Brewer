#include <fstream>
#include <Brewer/Brewer.hpp>
#include <Brewer/Context.hpp>
#include <Brewer/Module.hpp>
#include <Brewer/Parser.hpp>

int main()
{
    std::ifstream stream("example/example.b");
    Brewer::Context context;
    Brewer::Module dest(context);
    Brewer::Parser::Parse(stream, dest);
    stream.close();
}

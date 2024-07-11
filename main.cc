
#include "AtmoLexer.hh"
#include "parser.tab.hh"
#include <fstream>
// https://stackoverflow.com/questions/76509844/how-do-you-interface-c-flex-with-c-bison
int main(int argc, char** argv)
{
    AtmoLexer lexer;
    std::ifstream input("test.txt");
    lexer.switch_streams(&input);
    if (argc > 1)
    {
         lexer.set_debug(1);
    }
    yy::parser parser(lexer);
    return parser();

    return 0;
}
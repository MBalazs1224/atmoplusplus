
#include "AtmoLexer.hh"
#include "parser.tab.hh"

// https://stackoverflow.com/questions/76509844/how-do-you-interface-c-flex-with-c-bison

int main()
{
    AtmoLexer lexer;
    yy::parser parser(lexer);
    return parser();
}
#include "TestLexer.hh"
#include "testParser.tab.hh"

// https://stackoverflow.com/questions/76509844/how-do-you-interface-c-flex-with-c-bison

int main()
{
    TestLexer lexer;
    yy::parser parser(lexer);
    return parser();
}
#pragma once

#include <string>
#include <sstream>
#include <stack>
/*

#if ! defined(yyFlexLexerOnce) & #endif - We use the Flex mechanism of ensuring that the header <lexLexer.h> is added only once. (This is a little non-standard solution but allows us to include it multiple times if there is a need for that.)

int yylex(yylval_type yylval); - We do declare the function but the definition is provided by Flex.

*/

#if ! defined(yyFlexLexerOnce)
#include "FlexLexer.h"
#endif

#include "parser.tab.hh"
#include "location.hh"
#include <memory>

class test
    {
    public:
        int ival;
        double dval;
        std::string sval;
        char cval;
    };

class AtmoLexer : public yyFlexLexer
{
    private:
        std::unique_ptr<yy::parser::semantic_type> yylval = nullptr;
        std::unique_ptr<yy::parser::location_type> loc = nullptr;
    

    public:
    AtmoLexer(std::istream* in) : yyFlexLexer(in)
    {
        loc = std::make_unique<yy::parser::location_type>();
    }
    // Flex will provide the definition of this function, we just need to declare it
    int yylex(yy::parser::semantic_type* lval, yy::parser::location_type* location);
};
#pragma once

#include <string>

/*

#if ! defined(yyFlexLexerOnce) & #endif - We use the Flex mechanism of ensuring that the header <lexLexer.h> is added only once. (This is a little non-standard solution but allows us to include it multiple times if there is a need for that.)

int yylex(std::string *const yylval); - We do declare the function but the definition is provided by Flex.

*/

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

class TestLexer : public yyFlexLexer
{
    public:
    // Flex will provide the definition of this function, we just need to declare it
    int yylex(std::string *const yylval);
};
%{
    #include "AtmoLexer.hh"
    #include "parser.tab.hh"
    /*

    #undef  YY_DECL - C++ Flex still makes heavy use of macros. YY_DECL stores the declaration of function yylval that it will generate. We remove the default value, which is int FooLexer::yylex().
#define YY_DECL int FooLexer::yylex(std::string *const lval) - Now, we replace the removed value with the function declaration we need.

    */

    #undef YY_DECL
    #define YY_DECL int AtmoLexer::yylex(std::string *const yylval)
%}
/*

%option c++ - We switch the output language to C++.

%option yyclass="FooLexer" - Finally, we set which class should be used by lexer instead of the yyFlexLexer. It will create the method yylex in this class.

*/
%option noyywrap c++
%option yyclass="AtmoLexer"

%x STRING_LITERAL_TOKEN
%x COMMENT_TOKEN
%x CHAR_LITERAL_TOKEN
letter [A-Za-z_]
digit [0-9]
less_than less[ ]than|<
greater_than greater[ ]than|>
equals equals|=
plus plus|\+
minus minus|\-
times times|\*
divide divide(d)*[ ]by|\/
array_of array[ ]of|ARRAY[ ]OF
value_at (value[ ]at)|(VALUE[ ]AT)|@
address_of (address[ ]of)|(ADDRESS[ ]OF)

%%

Hello {return yy::parser::token::HELLO;}
[[:alpha:]]+ {*yylval = std::string(yytext,yytext + yyleng); return yy::parser::token::WORLD;}
. {return yytext[0];}

%%


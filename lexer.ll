%{
    #include "AtmoLexer.hh"
    #include "parser.tab.hh"

    /*

    #undef  YY_DECL - C++ Flex still makes heavy use of macros. YY_DECL stores the declaration of function yylval that it will generate. We remove the default value, which is int FooLexer::yylex().
#define YY_DECL int FooLexer::yylex(std::string *const lval) - Now, we replace the removed value with the function declaration we need.

    */

    #undef YY_DECL
    #define YY_DECL int AtmoLexer::yylex(test* yylval)

%}
/*

%option c++ - We switch the output language to C++.

%option yyclass="FooLexer" - Finally, we set which class should be used by lexer instead of the yyFlexLexer. It will create the method yylex in this class.

*/
%option noyywrap c++
%option yyclass="AtmoLexer"
%option debug

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
\t		{return yy::parser::token::TABULATOR;}
\n		{ return yy::parser::token::NEW_LINE;}
[ ]+ {/* skip whitespace */ }


\" {;BEGIN STRING_LITERAL_TOKEN;}
<STRING_LITERAL_TOKEN>\" {  BEGIN INITIAL; yylval->sval = str_buffer.str();
str_buffer.str("");
str_buffer.clear();
return yy::parser::token::STRING_LITERAL;}
<STRING_LITERAL_TOKEN><<EOF>>  {std::cout << "Unclosed string literal"; exit(1);}
<STRING_LITERAL_TOKEN>\\n {str_buffer << "\n";}
<STRING_LITERAL_TOKEN>\\t {str_buffer << "\t";}
<STRING_LITERAL_TOKEN>\\\" {str_buffer << "\"";}
<STRING_LITERAL_TOKEN>\\\\ {str_buffer << "\\";}
<STRING_LITERAL_TOKEN>\t { std::cout << "Invalid tabulator inside string literal"; exit(1);}
<STRING_LITERAL_TOKEN>\n { std::cout << "Invalid new line inside string literal"; exit(1);}
<STRING_LITERAL_TOKEN>[^\"] {str_buffer << YYText();}



' {BEGIN CHAR_LITERAL_TOKEN;}
<CHAR_LITERAL_TOKEN>\t {std::cout << "Invalid tabulator inside character literal" << std::endl ; exit(1);}
<CHAR_LITERAL_TOKEN>\n {std::cout << "Invalid new line inside character literal" << std::endl ; exit(1);}
<CHAR_LITERAL_TOKEN>\\t' {yylval->cval = '\t';BEGIN INITIAL; return yy::parser::token::CHAR_LITERAL;}
<CHAR_LITERAL_TOKEN>\\n' {yylval->cval = '\n';BEGIN INITIAL; return yy::parser::token::CHAR_LITERAL;}
<CHAR_LITERAL_TOKEN>.' {yylval->cval = yytext[0];BEGIN INITIAL; return yy::parser::token::CHAR_LITERAL;}
<CHAR_LITERAL_TOKEN>.{2,} {std::cout << "Too many characters inside character literal" << std::endl; exit(1);}
<CHAR_LITERAL_TOKEN><<EOF>> {std::cout << "Unclosed character literal" << std::endl ; exit(1);}



create|CREATE {return yy::parser::token::CREATE;}
function|FUNCTION { return yy::parser::token::FUNCTION; }
variable|VARIABLE  {return yy::parser::token::VARIABLE; }
((integer)s*)|((INTEGER)S*) { return yy::parser::token::INT;}
((string)s*)|((STRING)S*)  {return yy::parser::token::STRING;}
((float)s*)|((FLOAT)S*)   {  return yy::parser::token::FLOAT;}
if|ELSE		{ return yy::parser::token::IF;}
else|ELSE 	{ return yy::parser::token::ELSE;}
call|CALL { return yy::parser::token::CALL;}
not|NOT	{ return yy::parser::token::NOT;}
and|AND	{ return yy::parser::token::AND;}
or|OR	{ return yy::parser::token::OR;}
as|AS { return yy::parser::token::AS;}


","		{ return yy::parser::token::COMMA;}
"("		{ return yy::parser::token::OPEN_BRACKET;}
")"		{ return yy::parser::token::CLOSE_BRACKET;}
"["		{ return yy::parser::token::OPEN_SQUARE_BRACKET;}
"]"		{ return yy::parser::token::CLOSE_SQUARE_BRACKET;}


{less_than}		{ return yy::parser::token::LESS_THAN;}
{greater_than}		{ return yy::parser::token::GREATER_THAN;}
{plus}		{return yy::parser::token::PLUS;}
{minus}  {return yy::parser::token::MINUS;}
{equals} 	{return yy::parser::token::EQUALS;}
{times} 	{ return yy::parser::token::MULTIPLY;}
{divide} 	{ return yy::parser::token::DIVIDE;}
{value_at} {return yy::parser::token::VALUE_AT;}


{address_of} {return yy::parser::token::ADDRESS_OF;}
return|RETURN	{return yy::parser::token::RETURN;}
with|WITH	{return yy::parser::token::WITH;}
params|PARAMS	{  return yy::parser::token::PARAMS;}
matches|MATCHES		{ return yy::parser::token::MATCHES;}
until|UNTIL {return yy::parser::token::UNTIL;}
(char)s*|(CHAR)S* { return yy::parser::token::CHAR;}
class|CLASS {return yy::parser::token::CLASS;}
public|PUBLIC {return yy::parser::token::PUBLIC;}
private|PRIVATE {return yy::parser::token::PRIVATE;}
pointer|POINTER { return yy::parser::token::POINTER;}
inside|INSIDE {return yy::parser::token::INSIDE;}
true|TRUE {return yy::parser::token::TRUE;}
false|FALSE {return yy::parser::token::FALSE;}
(boolean)s*|(BOOLEAN)S* { return yy::parser::token::BOOLEAN;}
{array_of} {return yy::parser::token::ARRAY_OF;}


{letter}({letter}|{digit})* {yylval->sval = std::string(YYText());
return yy::parser::token::IDENTIFIER;}

{digit}+	 { yylval->ival = atoi(YYText()); return yy::parser::token::NUMBER;}

\.{digit}+f*|{digit}+\.{digit}+f*|{digit}+f*	{
    yylval->dval = atof(YYText());				
    return yy::parser::token::NUMBER_FLOAT;
}

{digit}+{letter}+  { std::cerr << "Invalid token: " << YYText();exit(1) ;}


. {std::cerr << "Invalid token: " << YYText();exit(1);}

%%


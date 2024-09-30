%{
    #include "AtmoLexer.hh"
    #include "parser.tab.hh"
    #include "src/symboltable/symboltable.hh"
    #include "src/symboltable/all_symbols.hh"
    #include <memory>
    /*

    #undef  YY_DECL - C++ Flex still makes heavy use of macros. YY_DECL stores the declaration of function yylval that it will generate. We remove the default value, which is int FooLexer::yylex().
#define YY_DECL int FooLexer::yylex(std::string *const lval) - Now, we replace the removed value with the function declaration we need.

    */

    #undef YY_DECL
    #define YY_DECL int AtmoLexer::yylex(yy::parser::semantic_type* const yylval, yy::parser::location_type* loc)

    // The stack used to keep track of the identation levels
    std::stack<int> ident_stack;

    // The current identation level
    int current_indent = 0;

    // The stringstream to temporarily store the state of string literals
    std::stringstream str_buffer;
    // Update location on every token match
    #define YY_USER_ACTION loc->step(); loc->columns(yyleng);

%}

    /* - We switch the output language to C++. */
%option c++ 
    /* - Finally, we set which class should be used by lexer instead of the yyFlexLexer. It will create the method yylex in this class. */
%option noyywrap c++
%option yyclass="AtmoLexer"
%option debug
%x IDENTATION
%x NORMAL
%x STRING_LITERAL_TOKEN
%x COMMENT_TOKEN
%x CHAR_LITERAL_TOKEN
letter [A-Za-z_]
digit [0-9]
less_than (less[ ]than)|(LESS[ ]THAN)<
less_than_or_equal (less[ ]than[ ]or[ ]equal)|(LESS[ ]THAN[ ]OR[ ]EQUAL)<=
greater_than greater[ ]than|(GREATER[ ]THAN)|>
greater_than_or_equal greater[ ]than[ ]or[ ]equal|(GREATER[ ]THAN[ ]OR[ ]EQUAL)|>=
equals equals|EQUALS|=
plus plus|PLUS|\+
minus minus|MINUS|\-
times times|TIMES|\*
divide divide(d)*[ ]by|(DIVIDE(D)*[ ]BY)|\/
array_of array[ ]of|ARRAY[ ]OF
do (do)|DO
void (void)|VOID
not_matches (not[ ]matches)|(NOT[ ]MATCHES)
protected (protected)|PROTECTED
static (static)|STATIC
derives_from (DERIVES[ ]FROM)|(derives[ ]from)|(DERIVED[ ]FROM)|(derived[ ]from)|(DERIVE[ ]FROM)|(derive[ ]from)
else_if  (ELSE[ ]IF)|(else[ ]if)
constructor  (constructor)|(CONSTRUCTOR)

%%
    // This code will be put into the top of yylex
    // to check if there are remeaning dedents
    static int dedents_remaining = 0;
    if(dedents_remaining > 0)
    {
        dedents_remaining--;
        return yy::parser::token::DEDENT; 
    }
    // FIXME: This probably could be improved so we don't have to reset the current identation level after every token
    current_indent = 0;

    // Normal state is needed so the identation scanning can be started on the first line


<NORMAL>\n		{ loc->lines();  ;BEGIN IDENTATION;}
<NORMAL>[ ]+ {/* skip whitespace */ }

<IDENTATION>\t {current_indent++;}
<IDENTATION>\n { loc->lines();  ; current_indent = 0;}
<IDENTATION>[^\t\n] {
    int previous_ident = ident_stack.empty() ? 0 : ident_stack.top();
    if(current_indent > previous_ident)
    {
        ident_stack.push(current_indent);
        BEGIN NORMAL;
        current_indent = 0;
        yyless(0);
        return yy::parser::token::INDENT;
    }
    else if(current_indent < previous_ident)
    {
        while(!ident_stack.empty() && current_indent < ident_stack.top())
        {
            ident_stack.pop();
            dedents_remaining++;

        }
        yyless(0);
        BEGIN NORMAL;
        dedents_remaining--;
        return yy::parser::token::DEDENT;
        
    }
    else
    {
        yyless(0);
        BEGIN NORMAL;
    }
    yyless(0);
    current_indent = 0;
}
<IDENTATION><<EOF>> {
    if(dedents_remaining > 0)
    {
        dedents_remaining--;
        return yy::parser::token::DEDENT; 
    }
    else if(!ident_stack.empty())
    {
        ident_stack.pop();
        return yy::parser::token::DEDENT;
    }
    else
    {
        yyterminate();
    }
    }
<NORMAL><<EOF>> {
    if(dedents_remaining > 0)
    {
        dedents_remaining--;
        return yy::parser::token::DEDENT; 
    }
    else if(!ident_stack.empty())
    {
        ident_stack.pop();
        return yy::parser::token::DEDENT;
    }
    else
    {
        yyterminate();
    }
}

<NORMAL>"//" { BEGIN COMMENT_TOKEN;}
<COMMENT_TOKEN>(\n) { loc->lines();  ;BEGIN NORMAL;}
<COMMENT_TOKEN>[^'"]"\\\\" {BEGIN NORMAL;}
<COMMENT_TOKEN>[^\n] { /* Skip the chars */}


<NORMAL>\" {;BEGIN STRING_LITERAL_TOKEN;}
<STRING_LITERAL_TOKEN>\" {  BEGIN NORMAL; yylval->emplace<std::string>(str_buffer.str());
str_buffer.str("");
str_buffer.clear();
return yy::parser::token::STRING_LITERAL;}
<STRING_LITERAL_TOKEN><<EOF>>  {
    Error::ShowError("Unclosed string literal!",*loc);
    exit(1);
    }
<STRING_LITERAL_TOKEN>\\n { loc->lines(); ;str_buffer << "\n";}
<STRING_LITERAL_TOKEN>\\t {str_buffer << "\t";}
<STRING_LITERAL_TOKEN>\\\" {str_buffer << "\"";}
<STRING_LITERAL_TOKEN>\\\\ {str_buffer << "\\";}
<STRING_LITERAL_TOKEN>\t { 
    Error::ShowError("Invalid tabulator inside string literal!",*loc);
    exit(1);}
<STRING_LITERAL_TOKEN>\n {  loc->lines();
Error::ShowError("Invalid new line inside string literal!",*loc);
 exit(1);}
<STRING_LITERAL_TOKEN>[^\"] {str_buffer << YYText();}



<NORMAL>' {BEGIN CHAR_LITERAL_TOKEN;}
<CHAR_LITERAL_TOKEN>\t {
    Error::ShowError("Invalid tabulator inside character literal!",*loc);
exit(1);}
<CHAR_LITERAL_TOKEN>\n { loc->lines();
Error::ShowError("Invalid new line inside character literal!",*loc);
exit(1);}
<CHAR_LITERAL_TOKEN>\\t' {
    yylval->emplace<char>('\t');
    BEGIN NORMAL; return yy::parser::token::CHAR_LITERAL;}
<CHAR_LITERAL_TOKEN>\\n' { loc->lines(); yylval->emplace<char>('\n');BEGIN NORMAL; return yy::parser::token::CHAR_LITERAL;}
<CHAR_LITERAL_TOKEN>.' {
yylval->emplace<char>(yytext[0]);
BEGIN NORMAL; return yy::parser::token::CHAR_LITERAL;}
<CHAR_LITERAL_TOKEN>.{2,} {
    Error::ShowError("Too many characters inside character literal!",*loc);
    exit(1);}
<CHAR_LITERAL_TOKEN><<EOF>> {
    Error::ShowError("Unclosed character literal!",*loc);
    exit(1);
    }



<NORMAL>create|CREATE {return yy::parser::token::CREATE;}
<NORMAL>function|FUNCTION { return yy::parser::token::FUNCTION; }
<NORMAL>variable|VARIABLE  {return yy::parser::token::VARIABLE; }
<NORMAL>((integer)s*)|((INTEGER)S*) { return yy::parser::token::INT;}
<NORMAL>((string)s*)|((STRING)S*)  {return yy::parser::token::STRING;}
<NORMAL>((float)s*)|((FLOAT)S*)   {  return yy::parser::token::FLOAT;}
<NORMAL>if|IF		{ return yy::parser::token::IF;}
<NORMAL>else|ELSE 	{ return yy::parser::token::ELSE;}
<NORMAL>call|CALL { return yy::parser::token::CALL;}
<NORMAL>not|NOT	{ return yy::parser::token::NOT;}
<NORMAL>and|AND	{ return yy::parser::token::AND;}
<NORMAL>or|OR	{ return yy::parser::token::OR;}
<NORMAL>as|AS { return yy::parser::token::AS;}


<NORMAL>","		{ return yy::parser::token::COMMA;}
<NORMAL>"("		{ return yy::parser::token::OPEN_BRACKET;}
<NORMAL>")"		{ return yy::parser::token::CLOSE_BRACKET;}
<NORMAL>"["		{ return yy::parser::token::OPEN_SQUARE_BRACKET;}
<NORMAL>"]"		{ return yy::parser::token::CLOSE_SQUARE_BRACKET;}


<NORMAL>{less_than}		{ return yy::parser::token::LESS_THAN;}
<NORMAL>{less_than_or_equal}		{ return yy::parser::token::LESS_THAN_OR_EQUAL;}
<NORMAL>{greater_than}		{ return yy::parser::token::GREATER_THAN;}
<NORMAL>{greater_than_or_equal}		{ return yy::parser::token::GREATER_THAN_OR_EQUAL;}
<NORMAL>{plus}		{return yy::parser::token::PLUS;}
<NORMAL>{minus}  {return yy::parser::token::MINUS;}
<NORMAL>{equals} 	{return yy::parser::token::EQUALS;}
<NORMAL>{times} 	{ return yy::parser::token::MULTIPLY;}
<NORMAL>{divide} 	{ return yy::parser::token::DIVIDE;}
<NORMAL>{do} {return yy::parser::token::DO;}
<NORMAL>{not_matches} {return yy::parser::token::NOT_MATCHES;}
<NORMAL>{void} {return yy::parser::token::VOID;}
<NORMAL>{static} {return yy::parser::token::STATIC;}
<NORMAL>{protected} {return yy::parser::token::PROTECTED;}
<NORMAL>{derives_from} {return yy::parser::token::DERIVES_FROM;}
<NORMAL>{else_if} {return yy::parser::token::ELSE_IF;}
<NORMAL>{constructor}	{return yy::parser::token::CONSTRUCTOR;}

<NORMAL>return|RETURN	{return yy::parser::token::RETURN;}
<NORMAL>with|WITH	{return yy::parser::token::WITH;}
<NORMAL>params|PARAMS	{  return yy::parser::token::PARAMS;}
<NORMAL>matches|MATCHES		{ return yy::parser::token::MATCHES;}
<NORMAL>until|UNTIL {return yy::parser::token::UNTIL;}
<NORMAL>(char)s*|(CHAR)S* { return yy::parser::token::CHAR;}
<NORMAL>class|CLASS {return yy::parser::token::CLASS;}
<NORMAL>public|PUBLIC {return yy::parser::token::PUBLIC;}
<NORMAL>private|PRIVATE {return yy::parser::token::PRIVATE;}
<NORMAL>inside|INSIDE {return yy::parser::token::INSIDE;}
<NORMAL>true|TRUE { yylval->emplace<bool>(true); return yy::parser::token::TRUE;}
<NORMAL>false|FALSE {yylval->emplace<bool>(false); return yy::parser::token::FALSE;}
<NORMAL>(boolean)s*|(BOOLEAN)S* { return yy::parser::token::BOOLEAN;}
<NORMAL>{array_of} {return yy::parser::token::ARRAY_OF;}


<NORMAL>{letter}({letter}|{digit})* {
    yylval->emplace<std::string>(std::string(YYText()));
    return yy::parser::token::IDENTIFIER;
}

<NORMAL>{digit}+	 { yylval->emplace<int>(atoi(YYText())); return yy::parser::token::NUMBER;}

<NORMAL>\.{digit}+f*|{digit}+\.{digit}+f*|{digit}+f*	{	
    yylval->emplace<double>(atof(YYText()));			
    return yy::parser::token::NUMBER_FLOAT;
}

<NORMAL>{digit}+{letter}+  { std::cerr << "Invalid token: " << YYText();exit(1) ;}


<NORMAL>. {std::cerr << "Invalid token: " << YYText();exit(1);}

. { yyless(0);BEGIN IDENTATION;}
\n { loc->lines(); yyless(0);BEGIN IDENTATION;}
%%


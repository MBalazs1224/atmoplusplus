%{
    #include "AtmoLexer.hh"
    #include "parser.tab.hh"
    #include "src/symboltable/SymbolTable.hh"
    #include "src/symboltable/Symbols.hh"
    #include <memory>
    /*

    #undef  YY_DECL - C++ Flex still makes heavy use of macros. YY_DECL stores the declaration of function yylval that it will generate. We remove the default value, which is int FooLexer::yylex().
#define YY_DECL int FooLexer::yylex(std::string *const lval) - Now, we replace the removed value with the function declaration we need.

    */

    #undef YY_DECL
    #define YY_DECL int AtmoLexer::yylex(test* yylval)

    // The stack used to keep track of the identation levels
    std::stack<int> ident_stack;

    // The current identation level
    int current_indent = 0;

    // The stringstream to temporarily store the state of string literals
    std::stringstream str_buffer;

%}
/*

%option c++ - We switch the output language to C++.

%option yyclass="FooLexer" - Finally, we set which class should be used by lexer instead of the yyFlexLexer. It will create the method yylex in this class.

*/
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
greater_than greater[ ]than|(GREATER[ ]THAN)|>
equals equals|EQUALS|=
plus plus|PLUS|\+
minus minus|MINUS|\-
times times|TIMES|\*
divide divide(d)*[ ]by|(DIVIDE(D)*[ ]BY)|\/
array_of array[ ]of|ARRAY[ ]OF
value_at (value[ ]at)|(VALUE[ ]AT)|@
address_of (address[ ]of)|(ADDRESS[ ]OF)
pointer_of (pointer[ ]of)|(POINTER[ ]OF)

%%
    // This code will be put into the top of yylex
    // to checkif there are remeaning dedents
    static int dedents_remeaning = 0;
    if(dedents_remeaning > 0)
    {
        dedents_remeaning--;
        std::cout << "Identation level popped! Remaning dedents: " << dedents_remeaning << std::endl;
        return yy::parser::token::DEDENT; 
    }

    // Normal state is needed so the identation scanning can be started on the first line


<NORMAL>\n		{ BEGIN IDENTATION;}
<NORMAL>[ ]+ {/* skip whitespace */ }

<IDENTATION>\t {current_indent++; /*std::cout << "identation incremented: " << current_indent << std::endl;*/}
<IDENTATION>\n { current_indent = 0;}
<IDENTATION>[^\t\n] {
    int previous_ident = ident_stack.empty() ? 0 : ident_stack.top();
    //std::cout << "Previous ident: " << previous_ident << std::endl;
    //std::cout << "Current ident: " << current_indent << std::endl;
    if(current_indent > previous_ident)
    {
        ident_stack.push(current_indent);
        std::cout << "New identation level pushed to stack: " << current_indent << std::endl;
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
            dedents_remeaning++;
        }
        yyless(0);
        BEGIN NORMAL;
        dedents_remeaning--;
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
    if(dedents_remeaning > 0)
    {
        dedents_remeaning--;
        std::cout << "Identation level popped!" << std::endl;
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
    if(dedents_remeaning > 0)
    {
        dedents_remeaning--;
        std::cout << "Identation level popped!" << std::endl;
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
<COMMENT_TOKEN>(\n) {BEGIN NORMAL;}
<COMMENT_TOKEN>[^'"]"\\\\" {BEGIN NORMAL;}
<COMMENT_TOKEN>[^\n] { /* Skip the chars */}


<NORMAL>\" {;BEGIN STRING_LITERAL_TOKEN;}
<STRING_LITERAL_TOKEN>\" {  BEGIN NORMAL; yylval->sval = str_buffer.str();
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



<NORMAL>' {BEGIN CHAR_LITERAL_TOKEN;}
<CHAR_LITERAL_TOKEN>\t {std::cout << "Invalid tabulator inside character literal" << std::endl ; exit(1);}
<CHAR_LITERAL_TOKEN>\n {std::cout << "Invalid new line inside character literal" << std::endl ; exit(1);}
<CHAR_LITERAL_TOKEN>\\t' {yylval->cval = '\t';BEGIN NORMAL; return yy::parser::token::CHAR_LITERAL;}
<CHAR_LITERAL_TOKEN>\\n' {yylval->cval = '\n';BEGIN NORMAL; return yy::parser::token::CHAR_LITERAL;}
<CHAR_LITERAL_TOKEN>.' {yylval->cval = yytext[0];BEGIN NORMAL; return yy::parser::token::CHAR_LITERAL;}
<CHAR_LITERAL_TOKEN>.{2,} {std::cout << "Too many characters inside character literal" << std::endl; exit(1);}
<CHAR_LITERAL_TOKEN><<EOF>> {std::cout << "Unclosed character literal" << std::endl ; exit(1);}



<NORMAL>create|CREATE {return yy::parser::token::CREATE;}
<NORMAL>function|FUNCTION { return yy::parser::token::FUNCTION; }
<NORMAL>variable|VARIABLE  {return yy::parser::token::VARIABLE; }
<NORMAL>((integer)s*)|((INTEGER)S*) { return yy::parser::token::INT;}
<NORMAL>((string)s*)|((STRING)S*)  {return yy::parser::token::STRING;}
<NORMAL>((float)s*)|((FLOAT)S*)   {  return yy::parser::token::FLOAT;}
<NORMAL>if|ELSE		{ return yy::parser::token::IF;}
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
<NORMAL>{greater_than}		{ return yy::parser::token::GREATER_THAN;}
<NORMAL>{plus}		{return yy::parser::token::PLUS;}
<NORMAL>{minus}  {return yy::parser::token::MINUS;}
<NORMAL>{equals} 	{return yy::parser::token::EQUALS;}
<NORMAL>{times} 	{ return yy::parser::token::MULTIPLY;}
<NORMAL>{divide} 	{ return yy::parser::token::DIVIDE;}
<NORMAL>{value_at} {return yy::parser::token::VALUE_AT;}
<NORMAL>{address_of} {return yy::parser::token::ADDRESS_OF;}
<NORMAL>{pointer_of} { return yy::parser::token::POINTER_OF;}


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
<NORMAL>true|TRUE {return yy::parser::token::TRUE;}
<NORMAL>false|FALSE {return yy::parser::token::FALSE;}
<NORMAL>(boolean)s*|(BOOLEAN)S* { return yy::parser::token::BOOLEAN;}
<NORMAL>{array_of} {return yy::parser::token::ARRAY_OF;}


<NORMAL>{letter}({letter}|{digit})* {
    yylval->sval = std::string(YYText());
    std::shared_ptr<VariableSymbol> ptr(new VariableSymbol(0,YYLeng(),0));
    SymbolTable::Insert(YYText(), ptr);
    return yy::parser::token::IDENTIFIER;
}

<NORMAL>{digit}+	 { yylval->ival = atoi(YYText()); return yy::parser::token::NUMBER;}

<NORMAL>\.{digit}+f*|{digit}+\.{digit}+f*|{digit}+f*	{
    yylval->dval = atof(YYText());				
    return yy::parser::token::NUMBER_FLOAT;
}

<NORMAL>{digit}+{letter}+  { std::cerr << "Invalid token: " << YYText();exit(1) ;}


<NORMAL>. {std::cerr << "Invalid token: " << YYText();exit(1);}

. { yyless(0);BEGIN IDENTATION;}
\n { yyless(0);BEGIN IDENTATION;}
%%


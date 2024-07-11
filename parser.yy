/*


    %require "3.2" - This directive not only makes sure the installed version of Bison supports C++. It also prevents creation of a redundant result file stack.hh.
    %language "c++" - We switch the output language to C++.
    %parse-param {FooLexer &lexer} - This directive adds an additional argument to the constructor of parser class. We use it to pass a lexer to the parser.
    #define yylex lexer.yylex - Parser still assumes that yylex is a global function. We use preprocessor to change that to a method of the lexer we're passing to the constructor.
    void yy::parser::error(const std::string &message) - We no longer need to declare the error handler at the beginning of the file. However, we still need to define it. The definition points now to a namespace yy and class parser which is the default location of the parser class.


*/


%require "3.2"
%language "c++"
%code requires {
    #include <string>
    #include "AtmoLexer.hh"
}
%define api.value.type {test}

%parse-param {AtmoLexer &lexer}

%header

%code {
    // lexer will be the name of the argument passed into the constructor of the parser
    #define yylex lexer.yylex
    #define YYDEBUG 1
    
}

%token CREATE
%token FUNCTION
%token VARIABLE
%token INT
%token STRING
%token IF
%token CALL
%token ELSE
%token COMMA
%token LESS_THAN
%token GREATER_THAN
%token TABULATOR
%token NEW_LINE
%token PLUS
%token MINUS
%token DIVIDE
%token MULTIPLY
%token EQUALS
%token NOT
%token AND
%token OR
%token RETURN
%token WITH
%token PARAMS
%token STRING_LITERAL
%token IDENTIFIER
%token FLOAT
%token MATCHES
%token UNTIL
%token CHAR_LITERAL
%token CHAR
%token CLASS
%token PUBLIC
%token PRIVATE
%token ARRAY_OF
%token NUMBER
%token NUMBER_FLOAT
%token INSIDE
%token OPEN_BRACKET
%token CLOSE_BRACKET
%token OPEN_SQUARE_BRACKET
%token CLOSE_SQUARE_BRACKET
%token POINTER
%token VALUE_AT
%token ADDRESS_OF
%token TRUE
%token FALSE
%token BOOLEAN
%token AS
%token INDENT
%token DEDENT

%left MINUS PLUS
%left MULTIPLY DIVIDE
%nonassoc UMINUS // For assigning minus numbers e.g -4 etc.
%%

program: %empty
        | statement_list
        | program INDENT
        | program DEDENT

statement_list: statement
                | statement_list statement
                 


statement: expression {std::cout << "expression was found" << std::endl;}
        | function_create
        | variable_definition 
        | variable_assignment

variable_assignment: IDENTIFIER EQUALS expression

variable_definition:CREATE DATATYPE IDENTIFIER equals_holder

equals_holder: %empty
                |EQUALS expression
            
function_create: CREATE FUNCTION IDENTIFIER argument_list INDENT statement_list DEDENT

argument_list: %empty
            | WITH argument
            | argument_list COMMA argument

argument: DATATYPE IDENTIFIER


DATATYPE: INT
          | BOOLEAN
          | STRING
          | FLOAT 

expression:  expression PLUS expression
            | expression MINUS expression
            | expression MULTIPLY expression
            | expression DIVIDE expression
            | IDENTIFIER
            | NUMBER
            | NUMBER_FLOAT
            | CHAR_LITERAL
            | STRING_LITERAL
            | TRUE
            | FALSE
            
%%

void yy::parser::error(const std::string &message)
{
    std::cerr << "Error: " << message << std::endl;
}
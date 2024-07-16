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
%token AND
%token OR
%token RETURN
%token WITH
%token PARAMS
%token STRING_LITERAL
%token IDENTIFIER
%token FLOAT
%token MATCHES
%token NOT_MATCHES
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
%token POINTER_OF
%token VALUE_AT
%token ADDRESS_OF
%token TRUE
%token FALSE
%token BOOLEAN
%token AS
%token INDENT
%token DEDENT
%token DO

%left MINUS PLUS
%left MULTIPLY DIVIDE AND OR GREATER_THAN LESS_THAN MATCHES NOT_MATCHES NOT
%nonassoc UMINUS // For assigning minus numbers e.g -4 etc.
%%


statement_list: statement
                | statement_list statement
                 


statement: expression
        | function_create
        | variable_definition 
        | variable_assignment
        | if_statement
        | until_statement
        | do_until_statement

variable_assignment: IDENTIFIER EQUALS expression

variable_definition:CREATE variable_type IDENTIFIER equals_holder



variable_type: DATATYPE
                | ARRAY_OF DATATYPE
                | POINTER_OF DATATYPE
equals_holder: %empty
                |EQUALS expression

body: INDENT statement_list DEDENT

do_until_statement: DO body UNTIL expression
until_statement: UNTIL expression body

if_statement: IF expression body else_statement

else_statement: %empty
                | ELSE body


function_call: CALL IDENTIFIER function_call_arguments

function_call_arguments: %empty
                        | WITH expression
                        | function_call_arguments COMMA expression

function_create: CREATE FUNCTION IDENTIFIER argument_list body

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
            | expression AND expression
            | expression OR expression
            | expression GREATER_THAN expression
            | expression LESS_THAN expression
            | expression MATCHES expression
            | expression NOT_MATCHES expression
            | OPEN_BRACKET expression CLOSE_BRACKET
            | NOT expression
            | IDENTIFIER
            | ADDRESS_OF IDENTIFIER
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
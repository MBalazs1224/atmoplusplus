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
    #include "TestLexer.hh"
}
%define api.value.type {std::string}

%parse-param {TestLexer &lexer}


%header

%code {
    // lexer will be the name of the argument passed into the constructor of the parser
    #define yylex lexer.yylex
}

%token HELLO
%token WORLD

%%

hello_world: HELLO WORLD '!' {std::cout << "Goodbye" << $WORLD << "!" << std::endl;}
%%

void yy::parser::error(const std::string &message)
{
    std::cerr << "Error: " << message << std::endl;
}
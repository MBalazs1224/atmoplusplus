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
    #include "src/symboltable/symboltable.hh"
    #include "src/symboltable/symbols.hh"
    #include "src/ast/nodes/all_nodes.hh"
    #include "src/ast/expressions/all_expressions.hh"
    #include <memory>
    #include <vector>
    #include <variant>
    #include "src/ast/literals/all_literals.hh"
    typedef std::variant<int, double, std::string,char> semantic_type;
    class AtmoDriver;
    class AtmoLexer;
}
%define api.value.type variant
%define         parse.trace
%locations

%parse-param {AtmoLexer &lexer}
%parse-param {AtmoDriver &driver}

%header

%code {

    #include "atmo_driver.hh"
    // lexer will be the name of the argument passed into the constructor of the parser
    #define yylex lexer.yylex
    #define YYDEBUG 1
    void* test = nullptr;
    
}
%define parse.error detailed
%define parse.lac full

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
%token LESS_THAN_OR_EQUAL
%token GREATER_THAN_OR_EQUAL
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
%token<std::string> STRING_LITERAL
%token IDENTIFIER
%token FLOAT
%token MATCHES
%token NOT_MATCHES
%token UNTIL
%token<char> CHAR_LITERAL
%token CHAR
%token CLASS
%token PUBLIC
%token PRIVATE
%token ARRAY_OF
%token<int> NUMBER
%token<double> NUMBER_FLOAT
%token INSIDE
%token OPEN_BRACKET
%token CLOSE_BRACKET
%token OPEN_SQUARE_BRACKET
%token CLOSE_SQUARE_BRACKET
%token POINTER_OF
%token VALUE_AT
%token ADDRESS_OF
%token<bool> TRUE
%token<bool> FALSE
%token BOOLEAN
%token AS
%token INDENT
%token DEDENT
%token DO

 //%left MINUS PLUS
 //%left MULTIPLY DIVIDE AND OR GREATER_THAN LESS_THAN MATCHES NOT_MATCHES NOT

%left OPEN_BRACKET CLOSE_BRACKET
%right NOT ADDRESS_OF
%left MULTIPLY DIVIDE
%left PLUS MINUS
%left GREATER_THAN GREATER_THAN_OR_EQUAL LESS_THAN LESS_THAN_OR_EQUAL
%left MATCHES NOT_MATCHES
%left OR
%left AND
%right EQUALS
%left COMMA

%nonassoc UMINUS // For assigning minus numbers e.g -4 etc.

%nterm<std::unique_ptr<StatementListNode>> statement_list
%nterm<std::unique_ptr<StatementNode>> statement
%nterm<std::unique_ptr<BodyNode>> body
%nterm<std::unique_ptr<UntilStatementNode>> until_statement
%nterm<std::unique_ptr<DoUntilStatementNode>> do_until_statement
%nterm<std::unique_ptr<IExpressionable>> expression
%%


statement_list: statement {$$ = std::make_unique<StatementListNode>(std::move($1));}
                | statement_list statement {
                    $1->Add(std::move($2));
                    $$ = std::move($1);
                }
                 


statement:function_create 
        | variable_definition 
        | variable_assignment 
        | if_statement 
        | until_statement 
        | do_until_statement
        | function_call

variable_assignment: IDENTIFIER EQUALS expression

variable_definition:CREATE variable_type IDENTIFIER equals_holder {
    // std::string id = $3.sval;
    // if(!SymbolTable::SymbolIsValid(id))
    // {
    //     std::cerr << "Error: " << id << " is not inside the symbol table!" << std::endl;
        
    // }
    // else
    // {
    //     if(SymbolTable::SymbolAlreadyDeclared(id))
    //     {
    //         std::cerr << "Error: multiple definiton of variable " << id << " !" << std::endl;
    //     }
    //     else
    //     {
    //         auto variableSymbol = std::make_shared<VariableSymbol>(0,0);
    //         SymbolTable::Switch(id,variableSymbol);
    //     }
    // }
   
}



variable_type: DATATYPE
                | ARRAY_OF DATATYPE
                | POINTER_OF DATATYPE
equals_holder: %empty
                |EQUALS expression

body: INDENT statement_list DEDENT {$$ = std::make_unique<BodyNode>(std::move($2)); }

do_until_statement: DO body UNTIL expression {$$ = std::make_unique<DoUntilStatementNode>(std::move($4),std::move($2));}
 //BUG: Until statement and body node sometimes get set to null
until_statement: UNTIL expression body {$$ = std::make_unique<UntilStatementNode>(std::move($2),std::move($3)); test = $$.get();
    // p ((UntilStatementNode*)test)->body->statementList->statements[1]
    }

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

    //FIXME: Expression precedence might need a rework, I tested it but I'm not really sure

expression:  expression PLUS expression {$$ = std::make_unique<AddExpression>(std::move($1),std::move($3));}
            | expression MINUS expression {$$ = std::make_unique<SubtractExpression>(std::move($1),std::move($3));}
            | expression MULTIPLY expression {$$ = std::make_unique<MultiplyExpression>(std::move($1),std::move($3));}
            | expression DIVIDE expression {$$ = std::make_unique<DivideExpression>(std::move($1),std::move($3));}
            | expression AND expression {$$ = std::make_unique<AndExpression>(std::move($1),std::move($3));}
            | expression OR expression {$$ = std::make_unique<OrExpression>(std::move($1),std::move($3));}
            | expression GREATER_THAN expression {$$ = std::make_unique<GreaterThanExpression>(std::move($1),std::move($3));}
            | expression LESS_THAN expression {$$ = std::make_unique<LessThanExpression>(std::move($1),std::move($3));}
            | expression GREATER_THAN_OR_EQUAL expression {$$ = std::make_unique<GreaterThanOrEqualExpression>(std::move($1),std::move($3));}
            | expression LESS_THAN_OR_EQUAL expression {$$ = std::make_unique<LessThanOrEqualExpression>(std::move($1),std::move($3));}
            | expression MATCHES expression {$$ = std::make_unique<MatchesExpression>(std::move($1),std::move($3));}
            | expression NOT_MATCHES expression {$$ = std::make_unique<NotMatchesExpression>(std::move($1),std::move($3));}
            | OPEN_BRACKET expression CLOSE_BRACKET {$$ = std::move($2);}
            // TODO: Make NotExpression take only one parameter instead of the null pointer
            | NOT expression {$$ = std::make_unique<NotExpression>(std::move($2), nullptr);}
            | dereference {$$ = nullptr;}
            | IDENTIFIER {$$ = nullptr;}
            | ADDRESS_OF IDENTIFIER {$$ = nullptr;}
            | VALUE_AT  {$$ = nullptr;}
            | NUMBER {$$ = std::make_unique<IntegerLiteral>($1);}
            | NUMBER_FLOAT {$$ = std::make_unique<FloatLiteral>($1);}
            | CHAR_LITERAL {$$ = std::make_unique<CharLiteral>($1);}
            | STRING_LITERAL {$$ = std::make_unique<StringLiteral>($1);}
            | TRUE {$$ = std::make_unique<BooleanLiteral>($1);}
            | FALSE {$$ = std::make_unique<BooleanLiteral>($1);}

dereference: VALUE_AT IDENTIFIER

%%

void yy::parser::error(const location_type &l, const std::string &message)
{
    std::cerr << "Error at " << l << ":" << message << std::endl;
}

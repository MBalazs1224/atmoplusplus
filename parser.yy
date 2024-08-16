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
    #include <sstream>
    #include "src/symboltable/symboltable.hh"
    #include "src/symboltable/all_symbols.hh"
    #include "src/ast/nodes/all_nodes.hh"
    #include "src/ast/expressions/all_expressions.hh"
    #include "src/ast/types/all_types.hh"
    #include <memory>
    #include <vector>
    #include "src/ast/literals/all_literals.hh"
    #include "src/ast/attributes/all_attributes.hh"
    #include "src/symboltable/argument.hh"
    class AtmoDriver;
    class AtmoLexer;
}
%define api.value.type variant
%define         parse.trace
 //%define         parse.lac           full

/* "unexpected" is unhelpful. "unexpected IDENTIFIER, expected NUMBER or STRING_LITERAL" is better. */
%define         parse.error         detailed

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

    //Type pointers so everything with the same type cah share them, not needing to create their owns (they need to be pointers so typeid can work correctly)

    std::shared_ptr<TypeInteger> TypeIntegerHolder = std::make_shared<TypeInteger>();
    std::shared_ptr<TypeFloat> TypeFloatHolder = std::make_shared<TypeFloat>();
    std::shared_ptr<TypeString> TypeStringHolder = std::make_shared<TypeString>();
    std::shared_ptr<TypeChar> TypeCharHolder = std::make_shared<TypeChar>();
    std::shared_ptr<TypeBoolean> TypeBooleanHolder = std::make_shared<TypeBoolean>();
    std::shared_ptr<TypeVoid> TypeVoidHolder = std::make_shared<TypeVoid>();

    std::shared_ptr<AttributePrivate> AttributePrivateHolder = std::make_shared<AttributePrivate>();
    std::shared_ptr<AttributePublic> AttributePublicHolder = std::make_shared<AttributePublic>();
    std::shared_ptr<AttributeProtected> AttributeProtectedHolder = std::make_shared<AttributeProtected>();
    std::shared_ptr<AttributeStatic> AttributeStaticHolder = std::make_shared<AttributeStatic>();

    
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
%token<std::string> IDENTIFIER
%token FLOAT
%token MATCHES
%token NOT_MATCHES
%token UNTIL
%token<char> CHAR_LITERAL
%token CHAR
%token CLASS
%token PUBLIC
%token PRIVATE
%token PROTECTED
%token STATIC
%token ARRAY_OF
%token<int> NUMBER
%token<double> NUMBER_FLOAT
%token INSIDE
%token OPEN_BRACKET
%token CLOSE_BRACKET
%token OPEN_SQUARE_BRACKET
%token CLOSE_SQUARE_BRACKET
%token<bool> TRUE
%token<bool> FALSE
%token BOOLEAN
%token AS
%token INDENT
%token DEDENT
%token DO
%token VOID

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
%nterm<std::unique_ptr<ReturnStatementNode>> return_statement
%nterm<std::unique_ptr<IfStatementNode>> if_statement
%nterm<std::unique_ptr<ElseStatementNode>> else_statement
    /*FIXME:This has to be a shared_ptr because of variableSymbols, might be improved*/
%nterm<std::shared_ptr<IExpressionable>> expression
%nterm<Type> datatype
%nterm<Type> variable_type
%nterm<Type> function_return_type
%nterm<std::shared_ptr<Attribute>> attribute
%nterm<std::shared_ptr<Argument>> argument
%nterm<std::vector<std::shared_ptr<VariableSymbol>>> argument_list
%nterm<std::shared_ptr<FunctionCall>> function_call
%nterm<std::vector<std::shared_ptr<IExpressionable>>> function_call_arguments


 //TODO: Temporary definitions so I can test individually
%nterm<std::unique_ptr<StatementNode>> function_create
%nterm<std::unique_ptr<StatementNode>> variable_definition
%nterm<std::unique_ptr<StatementNode>> variable_assignment


%%


    //BUG: All statements gets added to root (even if they are in a body etc..)
statement_list: statement {$$ = std::make_unique<StatementListNode>(std::move($1));}
                | statement_list statement {
                    $1->Add(std::move($2));
                    $$ = std::move($1);
                    test = $$.get();
                }
                 


statement:function_create {$$ = nullptr;}
        | variable_definition {$$ = nullptr;} 
        | variable_assignment {$$ = std::move($1);} 
        | if_statement  {$$ = std::move($1);}
        | until_statement  {$$ = std::move($1);}
        | do_until_statement  {$$ = std::move($1);}
        | return_statement {$$ = std::move($1);}

variable_assignment: IDENTIFIER EQUALS expression
{
    auto test = $3;
    $$ = std::make_unique<VariableAssignmentNode>(SymbolTable::LookUp($1),std::move($3));
}
variable_type: datatype {$$ = std::move($1);}
                | ARRAY_OF datatype {$2.SetIsArray(true); $$ = std::move($2);}

variable_definition:CREATE attribute variable_type IDENTIFIER equals_holder {
    auto variable = std::make_shared<VariableSymbol>(std::move($3),std::move($2));
    variable->location = @4;
    SymbolTable::Insert($4,std::move(variable));
}
attribute: %empty {$$ = AttributePrivateHolder;}
            | PUBLIC {$$ = AttributePublicHolder;}
            | PROTECTED {$$ = AttributeProtectedHolder;}
            | PRIVATE {$$ = AttributePrivateHolder;}
            | STATIC {$$ = AttributeStaticHolder;}

equals_holder: %empty
                |EQUALS expression


body: indent statement_list dedent {$$ = std::make_unique<BodyNode>($2->GetStatements()); }


indent: INDENT {SymbolTable::IncreaseScope();}
dedent: DEDENT {SymbolTable::DecreaseScope();}

do_until_statement: DO body UNTIL expression {$$ = std::make_unique<DoUntilStatementNode>(std::move($4),std::move($2));}

until_statement: UNTIL expression body {$$ = std::make_unique<UntilStatementNode>(std::move($2),std::move($3)); test = $$.get();
    }

if_statement: IF expression body else_statement {$$ = std::make_unique<IfStatementNode>(std::move($2),std::move($3),std::move($4)); test = $$.get(); /* TODO: Implement else if */}

else_statement: %empty
                | ELSE body {$$ = std::make_unique<ElseStatementNode>(std::move($2));}

return_statement: RETURN expression {$$ = std::make_unique<ReturnStatementNode>(std::move($2));}




function_create: CREATE attribute function_return_type FUNCTION IDENTIFIER argument_list body
{
    // Decrease the scope so the function will be inserted into the root, so everything can access it
    SymbolTable::DecreaseScope();
    //BUG: If the function is not added, the scopes and variables stay inside the SymbolTable
    if(!SymbolTable::IsRoot())
    {
        Error::ShowError("Functions can only be created on the root level!",@5);
    }
    else
    {
        auto temp = $6;
        auto functionSymbol = std::make_shared<FunctionSymbol>(std::move($3),std::move($2),$6,std::move($7));
        functionSymbol->location = @5;
        SymbolTable::Insert($5,functionSymbol);
    }
    

}
function_call: CALL IDENTIFIER function_call_arguments {
    auto function = SymbolTable::LookUp($2);
    $$ = std::make_shared<FunctionCall>(nullptr,nullptr, function,$3);
    }

function_call_arguments: %empty //TODO: Add params to the function call
                        

function_return_type: datatype {$$ = std::move($1);}
                    | VOID {$$ = TypeVoid();}

argument_list: %empty
            | WITH argument {
                // Increase the scope so the arguments can be pushed into their own scope
                SymbolTable::IncreaseScope();
                std::vector<std::shared_ptr<VariableSymbol>> args;

                auto variableSymbol = std::make_shared<VariableSymbol>($2->type,std::make_unique<AttributePrivate>());
                variableSymbol->location = @2;

                SymbolTable::Insert($2->name,variableSymbol);
                
                args.push_back(std::move(variableSymbol));
                $$ = args;
                }
            | argument_list COMMA argument {

                auto temp = $1;
                auto temp2 = $3;
                
                auto variableSymbol = std::make_shared<VariableSymbol>($3->type,std::make_unique<AttributePrivate>());
                variableSymbol->location = @3;
                $1.push_back(variableSymbol);
                SymbolTable::Insert($3->name,std::move(variableSymbol));
                $$ = $1;
            }

argument: datatype IDENTIFIER {$$ = std::make_shared<Argument>($2,std::move($1));}

datatype: INT { $$ = TypeInteger();}
          | BOOLEAN  { $$ = TypeBoolean();}
          | STRING  { $$ = TypeString();}
          | FLOAT  { $$ = TypeFloat();}
          | CHAR  { $$ = TypeChar();}


    //FIXME: Expression precedence might need a rework, I tested it but I'm not really sure
    // BUG: Expression location is not set correctly
expression:  expression PLUS expression {$$ = std::make_unique<AddExpression>(std::move($1),std::move($3));  }
            | expression MINUS expression {$$ = std::make_unique<SubtractExpression>(std::move($1),std::move($3));  }
            | expression MULTIPLY expression {$$ = std::make_unique<MultiplyExpression>(std::move($1),std::move($3));  }
            | expression DIVIDE expression {$$ = std::make_unique<DivideExpression>(std::move($1),std::move($3));  }
            | expression AND expression {$$ = std::make_unique<AndExpression>(std::move($1),std::move($3));  }
            | expression OR expression {$$ = std::make_unique<OrExpression>(std::move($1),std::move($3));  }
            | expression GREATER_THAN expression {$$ = std::make_unique<GreaterThanExpression>(std::move($1),std::move($3));  }
            | expression LESS_THAN expression {$$ = std::make_unique<LessThanExpression>(std::move($1),std::move($3));  }
            | expression GREATER_THAN_OR_EQUAL expression {$$ = std::make_unique<GreaterThanOrEqualExpression>(std::move($1),std::move($3));  }
            | expression LESS_THAN_OR_EQUAL expression {$$ = std::make_unique<LessThanOrEqualExpression>(std::move($1),std::move($3));  }
            | expression MATCHES expression {$$ = std::make_unique<MatchesExpression>(std::move($1),std::move($3));  }
            | expression NOT_MATCHES expression {$$ = std::make_unique<NotMatchesExpression>(std::move($1),std::move($3));  }
            | OPEN_BRACKET expression CLOSE_BRACKET {$$ = std::move($2);}
            // TODO: Make NotExpression take only one parameter instead of the null pointer
            | NOT expression {$$ = std::make_unique<NotExpression>(std::move($2), nullptr);  $$->location = $2->location;}
            | IDENTIFIER { /* TODO:Implement variables as expressions */ $$ = SymbolTable::LookUp($1);}
            | NUMBER {$$ = std::make_unique<IntegerLiteral>($1); $$->location = @1;}
            | NUMBER_FLOAT {$$ = std::make_unique<FloatLiteral>($1); $$->location = @1;}
            | CHAR_LITERAL {$$ = std::make_unique<CharLiteral>($1); $$->location = @1;}
            | STRING_LITERAL {$$ = std::make_unique<StringLiteral>($1); $$->location = @1;}
            | TRUE {$$ = std::make_unique<BooleanLiteral>($1); $$->location = @1;}
            | FALSE {$$ = std::make_unique<BooleanLiteral>($1); $$->location = @1;}
            | function_call {$$ = $1;}

%%

void yy::parser::error(const location_type &l, const std::string &message)
{
    std::cerr << "Error at " << l << ":" << message << std::endl;
}

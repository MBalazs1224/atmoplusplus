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
    #include "../symboltable/symboltable.hh"
    #include "../symboltable/all_symbols.hh"
    #include "../ast/nodes/all_nodes.hh"
    #include "../ast/expressions/all_expressions.hh"
    #include "../ast/types/all_types.hh"
    #include <memory>
    #include <vector>
    #include "../ast/literals/all_literals.hh"
    #include "../ast/attributes/all_attributes.hh"
    #include "../symboltable/argument.hh"
    class AtmoDriver;
    class AtmoLexer;
}
%define api.value.type variant
 //This gives segfault immediately
 //%define         parse.lac           full

/* "unexpected" is unhelpful. "unexpected IDENTIFIER, expected NUMBER or STRING_LITERAL" is better. */
%define parse.error detailed
%locations

%parse-param {AtmoLexer &lexer}
%parse-param {AtmoDriver &driver}

%header

%code {

    #include "../driver/atmo_driver.hh"
    // lexer will be the name of the argument passed into the constructor of the parser
    #define yylex lexer.yylex
    //#define YYDEBUG 1
    void* test = nullptr;

    std::shared_ptr<AttributePrivate> AttributePrivateHolder = std::make_shared<AttributePrivate>();
    std::shared_ptr<AttributePublic> AttributePublicHolder = std::make_shared<AttributePublic>();
    std::shared_ptr<AttributeProtected> AttributeProtectedHolder = std::make_shared<AttributeProtected>();
    std::shared_ptr<AttributeStatic> AttributeStaticHolder = std::make_shared<AttributeStatic>();

    // Will return the summed of the locations, but ignore if the pointer is empty
    yy::location AddLocations(std::shared_ptr<Node> exp_one, std::shared_ptr<Node> exp_two)
    {
        yy::location sum;
        if(exp_one)
        {
            // Set the beginning and the end also, because += only sets the end
            sum = exp_one->location;
        }
        if(exp_two)
        {
            sum += exp_two->location; 
        }
        return sum;
    }
    // Used at return stmt, because that doesn't haave 2 expressions
    yy::location AddLocations(yy::location loc, std::shared_ptr<Node> exp_two)
    {
        yy::location sum = loc;
        if(exp_two)
        {
            sum += exp_two->location; 
        }
        return sum;
    }

    
}
%token CREATE
%token FUNCTION
%token VARIABLE
%token INT
%token STRING
%token IF
%token CALL
%token ELSE
%token ELSE_IF
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
%token DERIVES_FROM
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
%token END_OF_FILE
%token CONSTRUCTOR
%token PARENT

%left MATCHES NOT_MATCHES
%right NOT
%right EQUALS
%left WITH
%right CALL
%right INSIDE
%left OPEN_BRACKET CLOSE_BRACKET
%left OR
%left AND
%left GREATER_THAN GREATER_THAN_OR_EQUAL LESS_THAN LESS_THAN_OR_EQUAL
%left PLUS MINUS
%left MULTIPLY DIVIDE
%right ARRAY_OF


%left COMMA

%nonassoc UMINUS // For assigning minus numbers e.g -4 etc.

%nterm<std::shared_ptr<StatementListNode>> statement_list
%nterm<std::shared_ptr<Node>> statement
%nterm<std::unique_ptr<BodyNode>> body
%nterm<std::unique_ptr<UntilStatementNode>> until_statement
%nterm<std::unique_ptr<DoUntilStatementNode>> do_until_statement
%nterm<std::unique_ptr<ReturnStatementNode>> return_statement
%nterm<std::unique_ptr<IfStatementNode>> if_statement
%nterm<std::unique_ptr<ElseStatementNode>> else_statement
%nterm<std::shared_ptr<IExpressionable>> expression
%nterm<std::shared_ptr<IExpressionable>> equals_holder
%nterm<std::shared_ptr<Type>> datatype
%nterm<std::shared_ptr<Type>> variable_type
%nterm<std::shared_ptr<Type>> function_return_type
%nterm<std::shared_ptr<Attribute>> attribute
%nterm<std::shared_ptr<Argument>> argument
%nterm<std::vector<std::shared_ptr<VariableSymbol>>> argument_list
%nterm<std::shared_ptr<FunctionCall>> function_call
%nterm<std::vector<std::shared_ptr<IExpressionable>>> function_call_arguments
%nterm<std::vector<std::shared_ptr<IExpressionable>>> more_arguments
%nterm<std::vector<std::shared_ptr<Identifier>>> base_classes
%nterm<std::unique_ptr<ElseIfStatementNode>> else_if_statement
%nterm<std::vector<std::unique_ptr<ElseIfStatementNode>>> else_if_statements
%nterm<std::vector<std::unique_ptr<ElseIfStatementNode>>> else_if_list
%nterm<std::shared_ptr<ConstructorDefinitionNode>> constructor_definition
%nterm<std::shared_ptr<ClassDefinitionNode>> class_create
%nterm<std::unique_ptr<Node>> function_create
%nterm<std::unique_ptr<Node>> variable_definition

%nterm<std::vector<std::shared_ptr<IExpressionable>>> parent_costructor_call

    /* TODO: Implement error recovery */


%%

program: statement_list  {
    auto test = $1;
    driver.ast_root = std::move($1);
    }

statement_list: statement {
                    $$ = std::make_shared<StatementListNode>(std::move($1));
                }
                | statement_list statement {
                    if($2)
                    {
                        $1->Add(std::move($2));
                    }
                    $$ = $1;
                }
                 


statement:function_create {$$ = std::move($1);}
        | class_create  {$$ = std::move($1);}
        | variable_definition {$$ = std::move($1);}
        | if_statement  {$$ = std::move($1);}
        | until_statement  {$$ = std::move($1);}
        | do_until_statement  {$$ = std::move($1);}
        | return_statement {$$ = std::move($1);}
        | expression {$$ = std::move($1);}
        | constructor_definition {$$ = std::move($1);}

variable_type: datatype {$$ = std::move($1);}

variable_definition:CREATE attribute variable_type IDENTIFIER equals_holder function_call_arguments {
    auto variable = std::make_shared<VariableSymbol>(std::move($3),std::move($2));
    variable->location = @4;
    SymbolTable::Insert($4,variable);
    auto s = $6;
    $$ = std::make_unique<VariableDefinitionNode>(std::move(variable), $5,AddLocations(variable, $5), $6);
} 
attribute: %empty {$$ = AttributePrivateHolder;}
            | PUBLIC {$$ = AttributePublicHolder;}
            | PROTECTED {$$ = AttributeProtectedHolder;}
            | PRIVATE {$$ = AttributePrivateHolder;}
            | STATIC {$$ = AttributeStaticHolder;}

equals_holder: %empty {}
                | EQUALS expression {$$ = $2;}


body: indent statement_list dedent {$$ = std::make_unique<BodyNode>($2->GetStatements()); }
        | %empty {
            $$ = std::make_unique<BodyNode>();
        }


indent: INDENT {SymbolTable::IncreaseScope();}
dedent: DEDENT {SymbolTable::DecreaseScope();}

do_until_statement: DO body UNTIL expression {$$ = std::make_unique<DoUntilStatementNode>(std::move($4),std::move($2),@1);}

until_statement: UNTIL expression body {$$ = std::make_unique<UntilStatementNode>(std::move($2),std::move($3),@1); test = $$.get();
    }


if_statement: IF expression body
else_if_statements else_statement {$$ = std::make_unique<IfStatementNode>(std::move($2),std::move($3),std::move($4),std::move($5),@1); test = $$.get(); }

else_statement: %empty {}
                | ELSE body {$$ = std::make_unique<ElseStatementNode>(std::move($2),@1);}
else_if_statements:
    %empty {}
    | else_if_list {$$ = std::move($1);}
;

else_if_list:
    else_if_statement {
        std::vector<std::unique_ptr<ElseIfStatementNode>> temp;
        temp.push_back(std::move($1));
        $$ = std::move(temp);
    }
    | else_if_list else_if_statement
    {
        $1.push_back(std::move($2));
        $$ = std::move($1);
    }
;

else_if_statement:
    ELSE_IF expression body {
        $$ = std::make_unique<ElseIfStatementNode>(std::move($2),std::move($3),@1);
    }
;

return_statement: RETURN expression {$$ = std::make_unique<ReturnStatementNode>($2,AddLocations(@1,$2));}

class_create: create_class_holder IDENTIFIER base_classes body
{

    auto symbol = std::make_shared<ClassSymbol>($3,std::move($4));
    symbol->location = @2;
    SymbolTable::Insert($2,symbol);
    $$ = std::make_unique<ClassDefinitionNode>(std::move(symbol));
}

/* Only single inheritance is allowed, but the parser will process more, so a correct error message can be shown by the semantic analyzer */ 

base_classes: %empty {}
            | DERIVES_FROM IDENTIFIER 
            {
                std::vector<std::shared_ptr<Identifier>> vec;
                auto id = std::make_shared<Identifier>(SymbolTable::LookUp($2),$2,@2);
                vec.push_back(id);

                $$ = std::move(vec);
            }
            | base_classes COMMA IDENTIFIER
            {

                auto id = std::make_shared<Identifier>(SymbolTable::LookUp($3),$3,@3);

                $1.push_back(id);

                $$ = std::move($1);
            }
            


create_class_holder: CREATE CLASS {/*Need to increase the scope so the class variables and functions have their own scope*/ //SymbolTable::IncreaseScope();
}

function_create: CREATE attribute function_return_type FUNCTION IDENTIFIER argument_list body
{

    // The argument list will increase the scope so the arguments can be inserted into their own scope, even if there are no arguments that's why we need to decrease it here too

    SymbolTable::DecreaseScope();

    //TODO: The semantics analyzer will have to check if the function is on the root
    auto functionSymbol = std::make_shared<FunctionSymbol>(std::move($3),std::move($2),$6,std::move($7));
    functionSymbol->location = @5;
    SymbolTable::Insert($5,functionSymbol);
    $$ = std::make_unique<FunctionDefinitionNode>(std::move(functionSymbol));
    $$->location = @5;

}
    // FIXME: Constructor definition might be unnecessary, can be implemented with an empty return type for funcion where the semantic analyzer will check if the function is a constructor
    
constructor_definition: CREATE attribute CONSTRUCTOR argument_list parent_costructor_call body
{
    // The argument list will increase the scope so the arguments can be inserted into their own scope, even if there are no arguments that's why we need to decrease it here too
    SymbolTable::DecreaseScope();

    auto function = std::make_shared<FunctionSymbol>(std::move($2),std::move($6),std::move($4));
    auto constructor = std::make_shared<ConstructorDefinitionNode>(std::move(function), std::move($5), @1 + @5);
    $$ = std::move(constructor);
    @$ = @1 + @5;
}

parent_costructor_call: %empty
                        {
                            std::vector<std::shared_ptr<IExpressionable>> empty;
                            $$ = empty;
                        }
                        | AND CALL PARENT function_call_arguments
                        {
                            $$ = $4;
                            @$ = @1 + @4;
                        }


function_call: CALL expression function_call_arguments {

    $$ = std::make_shared<FunctionCall>($2,$3,@1+@2);
    }

function_call_arguments: %empty {}
                        | WITH expression
                        {
                            std::vector<std::shared_ptr<IExpressionable>> vec;
                            vec.push_back($2);
                            $$ = std::move(vec);
                            @$ = @1 + @2;
                        }
                        | WITH expression more_arguments
                        {
                            // The first added expression should be on the front of the vector, so it can be compared to the function's wanted arguments


                            //FIXME: Inserting one element to the front of the vector will copy everything to the side which is an O(n) op, should be done with a better performance operation

                            $3.insert($3.begin(), $2);
                            $$ = std::move($3);
                            @$ = @1 + @3;
                        }
    // makes it clear that after the first argument, any next argument must be followed by a COMMA
more_arguments: COMMA expression
                {
                    std::vector<std::shared_ptr<IExpressionable>> vec;
                    vec.push_back(std::move($2));
                    $$ = std::move(vec);
                    @$ = @1 + @2;
                }
              | more_arguments COMMA expression
              {
                $1.push_back(std::move($3));
                $$ = std::move($1);
                @$ = @1 + @3;

              }

                        

function_return_type: datatype {$$ = std::move($1);}
                    | VOID {$$ = std::make_shared<TypeVoid>();;}

argument_list: %empty {
    // The function non terminal will decrease the scope no matter what, so we need to increase it here too
    SymbolTable::IncreaseScope();
}
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

datatype: INT { $$ = std::make_shared<TypeInteger>();}
          | BOOLEAN  { $$ = std::make_shared<TypeBoolean>();}
          | STRING  { $$ = std::make_shared<TypeString>();}
          | FLOAT  { $$ = std::make_shared<TypeFloat>();}
          | CHAR  { $$ = std::make_shared<TypeChar>();}
          | IDENTIFIER
          {
            auto id = std::make_shared<Identifier>(SymbolTable::LookUp($1),$1,@1);
            $$ = std::move(id);
          }
          | ARRAY_OF datatype
          {
            auto array = std::make_shared<Array>($2);
            $$ = std::move(array);
        }

expression:  expression PLUS expression {$$ = std::make_unique<AddExpression>( $1, $3, AddLocations($1,$3));  }
            | expression MINUS expression {$$ = std::make_unique<SubtractExpression>( $1, $3, AddLocations($1,$3));  }
            | expression MULTIPLY expression {$$ = std::make_unique<MultiplyExpression>( $1, $3, AddLocations($1,$3));  }
            | expression DIVIDE expression {$$ = std::make_unique<DivideExpression>( $1, $3, AddLocations($1,$3));  }
            | expression AND expression {$$ = std::make_unique<AndExpression>( $1, $3, AddLocations($1,$3));  }
            | expression OR expression {$$ = std::make_unique<OrExpression>( $1, $3, AddLocations($1,$3));  }
            | expression GREATER_THAN expression {$$ = std::make_unique<GreaterThanExpression>( $1, $3, AddLocations($1,$3));  }
            | expression LESS_THAN expression {$$ = std::make_unique<LessThanExpression>( $1, $3, AddLocations($1,$3));  }
            | expression GREATER_THAN_OR_EQUAL expression {$$ = std::make_unique<GreaterThanOrEqualExpression>( $1, $3, AddLocations($1,$3));  }
            | expression LESS_THAN_OR_EQUAL expression {$$ = std::make_unique<LessThanOrEqualExpression>( $1, $3, AddLocations($1,$3));  }
            | expression MATCHES expression {$$ = std::make_unique<MatchesExpression>( $1, $3, AddLocations($1,$3));  }
            | expression NOT_MATCHES expression {$$ = std::make_unique<NotMatchesExpression>( $1, $3, AddLocations($1,$3));  }
            | OPEN_BRACKET expression CLOSE_BRACKET {$$ =  $2;}
            | NOT expression {$$ = std::make_unique<NotExpression>( $2, @1 + $2->location); }
            | IDENTIFIER { 
                auto id = std::make_shared<Identifier>(SymbolTable::LookUp($1),$1,@1);
                $$ = std::move(id);
                }
            | NUMBER {$$ = std::make_unique<Literal<int,TypeInteger>>($1); $$->location = @1;}
            | NUMBER_FLOAT {$$ = std::make_unique<Literal<double,TypeFloat>>($1); $$->location = @1;}
            | CHAR_LITERAL {$$ = std::make_unique<Literal<char,TypeChar>>($1); $$->location = @1;}
            | STRING_LITERAL {$$ = std::make_unique<Literal<std::string, TypeString>>($1); $$->location = @1;}
            | TRUE {$$ = std::make_unique<Literal<bool,TypeBoolean>>($1); $$->location = @1;}
            | FALSE {$$ = std::make_unique<Literal<bool,TypeBoolean>>($1); $$->location = @1;}
            | function_call {$$ = $1;}
            | expression EQUALS expression
                {
                  $$ = std::make_unique<AssignmentExpression>( $1, $3, AddLocations($1,$3));
                }
		| expression INSIDE expression{
			$$ = std::make_unique<MemberAccessExpression>($1,$3, AddLocations($1,$3));}

%%

void yy::parser::error(const location_type &loc, const std::string &message)
{
    Error::ShowError(message, loc);
}

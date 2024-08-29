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

    //FIXME: Attributes might not need to be pointers

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
%nterm<std::vector<std::shared_ptr<ClassSymbol>>> base_classes
%nterm<std::unique_ptr<ElseIfStatementNode>> else_if_statement
%nterm<std::vector<std::unique_ptr<ElseIfStatementNode>>> else_if_statements


%nterm<std::unique_ptr<Node>> function_create
%nterm<std::unique_ptr<Node>> variable_definition


%%

statement_list: statement {$$ = std::make_unique<StatementListNode>(std::move($1));}
                | statement_list statement {
                    if($2)
                    {
                        $1->Add(std::move($2));
                    }
			test = $1.get();
                    $$ = std::move($1);
                }
                 


statement:function_create {$$ = std::move($1);}
        | class_create {$$ = nullptr;}
        | variable_definition {$$ = std::move($1);}
        | if_statement  {$$ = std::move($1);}
        | until_statement  {$$ = std::move($1);}
        | do_until_statement  {$$ = std::move($1);}
        | return_statement {$$ = std::move($1);}
        | expression {$$ = std::move($1);}


variable_type: datatype {$$ = std::move($1);}
                | ARRAY_OF datatype {$2->SetIsArray(true); $$ = std::move($2);}

variable_definition:CREATE attribute variable_type IDENTIFIER equals_holder {
    auto variable = std::make_shared<VariableSymbol>(std::move($3),std::move($2));
    variable->location = @4;
    SymbolTable::Insert($4,variable);
    $$ = std::make_unique<VariableDefinitionNode>(std::move(variable));
}
attribute: %empty {$$ = AttributePrivateHolder;}
            | PUBLIC {$$ = AttributePublicHolder;}
            | PROTECTED {$$ = AttributeProtectedHolder;}
            | PRIVATE {$$ = AttributePrivateHolder;}
            | STATIC {$$ = AttributeStaticHolder;}

equals_holder: %empty
                |EQUALS expression {$$ = $2;}


body: indent statement_list dedent {$$ = std::make_unique<BodyNode>($2->GetStatements()); }


indent: INDENT {SymbolTable::IncreaseScope();}
dedent: DEDENT {SymbolTable::DecreaseScope();}

do_until_statement: DO body UNTIL expression {$$ = std::make_unique<DoUntilStatementNode>(std::move($4),std::move($2),@1);}

until_statement: UNTIL expression body {$$ = std::make_unique<UntilStatementNode>(std::move($2),std::move($3),@1); test = $$.get();
    }


if_statement: IF expression body
else_if_statements else_statement {$$ = std::make_unique<IfStatementNode>(std::move($2),std::move($3),std::move($4),std::move($5),@1); test = $$.get(); }

else_statement: %empty
                | ELSE body {$$ = std::make_unique<ElseStatementNode>(std::move($2),@1);}

else_if_statement: ELSE_IF expression body
                {
                    $$ = std::make_unique<ElseIfStatementNode>(std::move($2),std::move($3),@1);

                }

else_if_statements: %empty
                | else_if_statement
                {
                    std::vector<std::unique_ptr<ElseIfStatementNode>> temp;
                    temp.push_back(std::move($1));
                    $$ = std::move(temp);

                }
                | else_if_statements else_if_statement
                {
                    
                    $$ = std::move($1);
                    $$.push_back(std::move($2));
                }

return_statement: RETURN expression {$$ = std::make_unique<ReturnStatementNode>(std::move($2),@1 + $2->location);}

class_create: create_class_holder IDENTIFIER base_classes body
{
    // Decrease the scope to get back to the root from the own scope of the class
    SymbolTable::DecreaseScope();
    if(!SymbolTable::IsRoot())
    {
        Error::ShowError("Classes can only be created on the root scope!",@2);
    }
    auto symbol = std::make_shared<ClassSymbol>($3,std::move($4));
    SymbolTable::Insert($2,symbol);
    
}
    /*FIXME: Might need to show error here if the given identifier is not class type, beacuse in the syntax analyzer, we wont have location or the given string jnfo*/
base_classes: %empty {}
            | DERIVES_FROM IDENTIFIER 
            {
                auto symbol = SymbolTable::LookUp($2);
                std::vector<std::shared_ptr<ClassSymbol>> vec;
                vec.push_back(std::dynamic_pointer_cast<ClassSymbol>(symbol));
                $$ = std::move(vec);
            }
            /*BUG: Multiple parents give back empty vector*/
            | base_classes COMMA IDENTIFIER
            {
                auto symbol = SymbolTable::LookUp($3);
                $$ = std::move($1);
                $$.push_back(std::dynamic_pointer_cast<ClassSymbol>(symbol));
                auto valami = $$;
            }
            


create_class_holder: CREATE CLASS {/*Need to increase the scope so the class variables and functions have their own scope*/ SymbolTable::IncreaseScope();}

function_create: CREATE attribute function_return_type FUNCTION IDENTIFIER argument_list body
{
    // Decrease the scope so the function will be inserted into the root, so everything can access it
    SymbolTable::DecreaseScope();
    //TODO: The semantics analyzer will have to check if the function is on the root
    auto functionSymbol = std::make_shared<FunctionSymbol>(std::move($3),std::move($2),$6,std::move($7));
    functionSymbol->location = @5;
    SymbolTable::Insert($5,functionSymbol);
    $$ = std::make_unique<FunctionDefinitionNode>(std::move(functionSymbol));

}
function_call: CALL IDENTIFIER function_call_arguments {
    auto function = SymbolTable::LookUp($2);
    $$ = std::make_shared<FunctionCall>(function,$3,@1+@2,$2);
    }

function_call_arguments: %empty //TODO: Add params to the function call
                        

function_return_type: datatype {$$ = std::move($1);}
                    | VOID {$$ = std::make_shared<TypeVoid>();;}

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

datatype: INT { $$ = std::make_shared<TypeInteger>();}
          | BOOLEAN  { $$ = std::make_shared<TypeBoolean>();}
          | STRING  { $$ = std::make_shared<TypeString>();}
          | FLOAT  { $$ = std::make_shared<TypeFloat>();}
          | CHAR  { $$ = std::make_shared<TypeChar>();}
          | IDENTIFIER
          {
            auto symbol = SymbolTable::LookUp($1);
            $$ = std::move(std::dynamic_pointer_cast<ClassSymbol>(symbol));
          }


    //FIXME: Expression precedence might need a rework, I tested it but I'm not really sure
    // BUG: Expression location is not set correctly
expression:  expression PLUS expression {$$ = std::make_unique<AddExpression>( $1, $3, $1->location + $3->location);  }
            | expression MINUS expression {$$ = std::make_unique<SubtractExpression>( $1, $3, $1->location + $3->location);  }
            | expression MULTIPLY expression {$$ = std::make_unique<MultiplyExpression>( $1, $3, $1->location + $3->location);  }
            | expression DIVIDE expression {$$ = std::make_unique<DivideExpression>( $1, $3, $1->location + $3->location);  }
            | expression AND expression {$$ = std::make_unique<AndExpression>( $1, $3, $1->location + $3->location);  }
            | expression OR expression {$$ = std::make_unique<OrExpression>( $1, $3, $1->location + $3->location);  }
            | expression GREATER_THAN expression {$$ = std::make_unique<GreaterThanExpression>( $1, $3, $1->location + $3->location);  }
            | expression LESS_THAN expression {$$ = std::make_unique<LessThanExpression>( $1, $3, $1->location + $3->location);  }
            | expression GREATER_THAN_OR_EQUAL expression {$$ = std::make_unique<GreaterThanOrEqualExpression>( $1, $3, $1->location + $3->location);  }
            | expression LESS_THAN_OR_EQUAL expression {$$ = std::make_unique<LessThanOrEqualExpression>( $1, $3, $1->location + $3->location);  }
            | expression MATCHES expression {$$ = std::make_unique<MatchesExpression>( $1, $3, $1->location + $3->location);  }
            | expression NOT_MATCHES expression {$$ = std::make_unique<NotMatchesExpression>( $1, $3, $1->location + $3->location);  }
            | OPEN_BRACKET expression CLOSE_BRACKET {$$ =  $2;}
            // TODO: Make NotExpression take only one parameter instead of the null pointer
            | NOT expression {$$ = std::make_unique<NotExpression>( $2, @1 + $2->location); }
            | IDENTIFIER { /* TODO:Implement variables as expressions */ $$ = SymbolTable::LookUp($1);}
            | NUMBER {$$ = std::make_unique<IntegerLiteral>($1); $$->location = @1;}
            | NUMBER_FLOAT {$$ = std::make_unique<FloatLiteral>($1); $$->location = @1;}
            | CHAR_LITERAL {$$ = std::make_unique<CharLiteral>($1); $$->location = @1;}
            | STRING_LITERAL {$$ = std::make_unique<StringLiteral>($1); $$->location = @1;}
            | TRUE {$$ = std::make_unique<BooleanLiteral>($1); $$->location = @1;}
            | FALSE {$$ = std::make_unique<BooleanLiteral>($1); $$->location = @1;}
            | function_call {$$ = $1;}
            | expression EQUALS expression
                {
                    // FIXME: This needs to be edited to ignore the location if it's null
                    yy::location loc = @2;
                    if($1)
                    {
                        loc += $1->location;
                    }
                    if($3)
                    {
                        loc += $3->location;
                    }
                    auto test = $3;
                    $$ = std::make_unique<AssignmentExpression>( $1, $3, loc);
                }
		| expression INSIDE expression{
			$$ = std::make_unique<MemberAccessExpression>($1,$3, @1 + @3);}

%%

void yy::parser::error(const location_type &l, const std::string &message)
{
    std::cerr << "Error at " << l << ":" << message << std::endl;
}

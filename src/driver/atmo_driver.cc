#include "atmo_driver.hh"

void AtmoDriver::StartCompilation(std::istream &stream)
{
    if (!stream.good() && stream.eof())
    {
        return;
    }

    lexer = CreateLexer(stream);

    try
    { 
        parser = std::make_unique<yy::parser>(*lexer, *this);
        parser->set_debug_level(parser_debug_level);
    }
    catch(const std::bad_alloc& ba)
    {
        std::cerr << "Failed to allocate parser: (" <<
         ba.what() << ")" << std::endl;
        exit(EXIT_FAILURE);
    }

    //TODO: Let the user choose input file, currently defaults to test.txt

    Error::Initialize();
    SymbolTable::Initialize();
    parser->parse();
    if (!Error::CanContinue())
    {
        std::cerr << "Parsing failed, exiting!" << std::endl;
        exit(EXIT_FAILURE);
    }
    SemanticAnalyze();

    if(!Error::CanContinue())
    {
        return;
    }

    TranslateToIR();

}

std::unique_ptr<AtmoLexer> AtmoDriver::CreateLexer(std::istream &stream)
{
    try
    {
        std::unique_ptr<AtmoLexer> lex = std::make_unique<AtmoLexer>(&stream);
        lex->set_debug(lexer_debug_level);
        return lex;
    }
    catch(const std::bad_alloc& ba)
    {
        std::cerr << "Failed to allocate scanner: (" <<
         ba.what() << ")" << std::endl;
        exit(EXIT_FAILURE);
    }
    return  nullptr;    
    
}

std::vector<std::shared_ptr<VariableSymbol>> AtmoDriver::GetGlobalVariables(std::vector<std::shared_ptr<Node>>& nodes)
{
    std::vector<std::shared_ptr<VariableSymbol>> globalVariables;

    for (auto &&node : nodes)
    {
        // If the variable was defined globally
        if(auto varDef = std::dynamic_pointer_cast<VariableDefinitionNode>(node))
        {
            globalVariables.push_back(varDef->GetVariable());
        }
        // If the variable was defined inside a nested scope
        else if(auto varContainer = std::dynamic_pointer_cast<VariableContainer>(node))
        {
            auto variables = varContainer->GetVariables();

            // Insert the variables into the end of the global vars
            globalVariables.insert(globalVariables.end(), variables.begin(), variables.end());
        }
        
    }
    
    return globalVariables;
}

std::shared_ptr<BoolList> AtmoDriver::GetWetherGlobalVariablesEscape(std::vector<std::shared_ptr<VariableSymbol>>& variables)
{
    // Global variables should be in the frame, so true will indicate that

    std::shared_ptr<BoolList> boolList = nullptr;

    for (size_t i = 0; i < variables.size(); i++)
    {
        boolList = std::make_shared<BoolList>(true, boolList);
    }

    return boolList;
    
}

void AtmoDriver::TranslateToIR()
{
    //Initialize rsp and rbp
    ReservedIrRegisters::Initialize();


    auto nodes = ast_root->GetStatementsRef();

    auto global_variables = GetGlobalVariables(nodes);

    x86Frame frame;

    //Create label for main

    auto mainLabel = Label("main");

    auto globalFrame = frame.newFrame(
        mainLabel,
        GetWetherGlobalVariablesEscape(global_variables)
    );

    // Set the variable's access to the access allocated by the frame object
    

    // FIXME: Variable's access gets allocated backwards (the second variables is offset 0, first is -8)
    
    auto accessInsideFrame = globalFrame->formals;

    int varCounter = 0;
    
    while (accessInsideFrame != nullptr)
    {
        // The received accessList will be backwards, so I have to iterate the global variables from backwards

        size_t currentIndexFromBackwards = global_variables.size() - 1 - varCounter++;
        
        global_variables[currentIndexFromBackwards]->access = accessInsideFrame->head;

        accessInsideFrame = accessInsideFrame->tail;
    }
    
    


    std::shared_ptr<IRStatementList> temp = nullptr;

    for (auto &&node : nodes)
    {
        auto ir = node->TranslateToIR();

        // FIXME: This is only needed until all IR translation is implemented
        if(ir == nullptr)
        {
            continue;
        }

        auto ir_list = std::make_shared<IRStatementList>(ir, nullptr);
        if(ir_root == nullptr)
        {
            ir_root = ir_list;
            temp = ir_root;
        }
        else
        {
            temp->tail = ir_list;
            temp = ir_list;
        }
    }
}


std::unique_ptr<yy::parser> AtmoDriver::CreateParser(const std::unique_ptr<AtmoLexer> &lexer)
{
    try
    {
        std::unique_ptr<yy::parser> parser = std::make_unique<yy::parser>(*lexer, *this);
        parser->set_debug_level(parser_debug_level);
        return parser;
    }
    catch(const std::bad_alloc& ba)
    {
        std::cerr << "Failed to allocate parser: (" <<
         ba.what() << ")" << std::endl;
        exit(EXIT_FAILURE);
    }
    return  nullptr;    
    
}

void AtmoDriver::parse_only(std::istream& stream)
{
    auto lexer = CreateLexer(stream);
    auto parser = CreateParser(std::move(lexer));
    Error::Initialize();
    SymbolTable::Initialize();
    parser->parse();
}

void AtmoDriver::SemanticAnalyze()
{
    for (auto statement : ast_root->GetStatementsRef())
    {
        if(!StatementValid(statement))
        {
            Error::ShowError("Invalid statement!",statement->location);
            continue;
        }
        statement->Check();
    }
    
}

bool AtmoDriver::StatementValid(const std::shared_ptr<Node> node)
{
    if(auto expression = std::dynamic_pointer_cast<IExpressionable>(node))
    {
        // Only assignment expressions and function calls can be used as a statement 

        return std::dynamic_pointer_cast<AssignmentExpression>(expression) != nullptr || std::dynamic_pointer_cast<FunctionCall>(expression) != nullptr;
    }

    // The only node not legal as a statement is constructor definition

    return std::dynamic_pointer_cast<ConstructorDefinitionNode>(node) == nullptr;
}

void AtmoDriver::set_parser_debug_level(int level)
{
    parser_debug_level = level;
}

void AtmoDriver::set_lexer_debug_level(int level)
{
    lexer_debug_level = level;
}
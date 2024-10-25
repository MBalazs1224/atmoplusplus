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
        if(StatementNotValid(statement))
        {
            Error::ShowError("Invalid statement!",statement->location);
            continue;
        }
        statement->Check();
    }
    
}

bool AtmoDriver::StatementNotValid(const std::shared_ptr<Node> node)
{
    if(auto expression = std::dynamic_pointer_cast<IExpressionable>(node))
    {
        // Only assignment expressions and function calls can be used as a statement 

        return std::dynamic_pointer_cast<AssignmentExpression>(expression) != nullptr && std::dynamic_pointer_cast<FunctionCall>(expression) != nullptr;
    }

    // The only node not legal as a statement is constructior definition

    return std::dynamic_pointer_cast<ConstructorDefinitionNode>(node) != nullptr;
}

void AtmoDriver::set_parser_debug_level(int level)
{
    parser_debug_level = level;
}

void AtmoDriver::set_lexer_debug_level(int level)
{
    lexer_debug_level = level;
}
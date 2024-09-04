#include "atmo_driver.hh"

void AtmoDriver::StartCompilation(std::istream &stream)
{
    if (!stream.good() && stream.eof())
    {
        return;
    }

    try
    {
        lexer = std::make_unique<AtmoLexer>(&stream);
        lexer->set_debug(lexer_debug_level);
    }
    catch(const std::bad_alloc& ba)
    {
        std::cerr << "Failed to allocate scanner: (" <<
         ba.what() << ")" << std::endl;
        exit(EXIT_FAILURE);
    }

    try
    { 
        parser = std::make_unique<yy::parser>(*lexer, *this, ast_root);
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

void AtmoDriver::SemanticAnalyze()
{
    for (auto statement : ast_root->GetStatementsRef())
    {
        statement->Check();
    }
    
}

void AtmoDriver::set_parser_debug_level(int level)
{
    parser_debug_level = level;
}

void AtmoDriver::set_lexer_debug_level(int level)
{
    lexer_debug_level = level;
}
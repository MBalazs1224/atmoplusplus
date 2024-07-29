#include "atmo_driver.hh"

void AtmoDriver::parse(std::istream &stream)
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
        parser = std::make_unique<yy::parser>(*lexer, *this);
        parser->set_debug_level(parser_debug_level);
    }
    catch(const std::bad_alloc& ba)
    {
        std::cerr << "Failed to allocate parser: (" <<
         ba.what() << ")" << std::endl;
        exit(EXIT_FAILURE);
    }


    int accept_code = 0;
    if (parser->parse() != accept_code)
    {
        std::cerr << "Parse failed!" << std::endl;
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
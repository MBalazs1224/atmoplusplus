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
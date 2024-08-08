#include <istream>
#include <memory>

#include "AtmoLexer.hh"
#include "parser.tab.hh"
class AtmoDriver
{
    public:
    AtmoDriver() = default;
    ~AtmoDriver() = default;
    void parse(std::istream &iss);
    void set_parser_debug_level(int);
    void set_lexer_debug_level(int);

    private:
    
    int lexer_debug_level = 0;
    int parser_debug_level = 0;
    
    std::unique_ptr<AtmoLexer> lexer = nullptr;
    std::unique_ptr<yy::parser> parser = nullptr;

};
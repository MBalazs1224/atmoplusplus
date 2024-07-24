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
    private:
    
    
    std::unique_ptr<AtmoLexer> lexer = nullptr;
    std::unique_ptr<yy::parser> parser = nullptr;

};
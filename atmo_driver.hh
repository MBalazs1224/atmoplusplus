#include <istream>
#include <memory>
#include <iostream>
#include "AtmoLexer.hh"
#include "parser.tab.hh"
class AtmoDriver
{
    public:
    AtmoDriver() = default;
    ~AtmoDriver() = default;
    void StartCompilation(std::istream &iss);
    void set_parser_debug_level(int);
    void set_lexer_debug_level(int);
    std::shared_ptr<StatementListNode> ast_root;


    private:
    void SemanticAnalyze();
    
    int lexer_debug_level = 0;
    int parser_debug_level = 0;
    
    std::unique_ptr<AtmoLexer> lexer = nullptr;
    std::unique_ptr<yy::parser> parser = nullptr;

};
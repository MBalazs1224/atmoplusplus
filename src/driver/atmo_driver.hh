#include <istream>
#include <memory>
#include <iostream>
#include "../lexer/AtmoLexer.hh"
#include "../parser/parser.tab.hh"

#include "../ir/statements/ir_statement_list.hh"
class AtmoDriver
{
    public:
    AtmoDriver() = default;
    ~AtmoDriver() = default;
    void StartCompilation(std::istream &iss);
    void set_parser_debug_level(int);
    void set_lexer_debug_level(int);
    std::shared_ptr<StatementListNode> ast_root;
    std::unique_ptr<AtmoLexer> CreateLexer(std::istream&);
    std::unique_ptr<yy::parser> CreateParser(const std::unique_ptr<AtmoLexer>&);

    
    // Will only build the AST based on the given stream (used during testing)
    void parse_only(std::istream&);

    private:

    std::shared_ptr<IRStatementList> ir_root;;

    void SemanticAnalyze();

    void TranslateToIR();
    
    // Will return tru if the given node can be used as a statement
    bool StatementValid(const std::shared_ptr<Node>);
    int lexer_debug_level = 0;
    int parser_debug_level = 0;
    
    std::unique_ptr<AtmoLexer> lexer = nullptr;
    std::unique_ptr<yy::parser> parser = nullptr;

};
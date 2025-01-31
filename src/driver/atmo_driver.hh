#include <istream>
#include <memory>
#include <iostream>
#include <utility>
#include "../lexer/AtmoLexer.hh"
#include "../parser/parser.tab.hh"

#include "../ir/statements/ir_statement_list.hh"
#include "../ir/reserved_ir_registers.hh"
class AtmoDriver
{

    private:
    std::vector<std::shared_ptr<VariableSymbol>> GetGlobalVariables(std::vector<std::shared_ptr<Node>>&);
    
    std::shared_ptr<BoolList> GetWetherGlobalVariablesEscape(std::vector<std::shared_ptr<VariableSymbol>>& );
    
    public:
    AtmoDriver() = default;
    ~AtmoDriver() = default;
    void StartCompilation(std::istream &iss);
    void set_parser_debug_level(int);
    void set_lexer_debug_level(int);
    std::shared_ptr<StatementListNode> ast_root;
    std::unique_ptr<AtmoLexer> CreateLexer(std::istream&);
    std::unique_ptr<yy::parser> CreateParser(const std::unique_ptr<AtmoLexer>&);

    // A vector that will hold the labels and values to all strings in the program
    static std::vector<std::pair<std::shared_ptr<Label>,std::string>> global_strings;
    
    // Will only build the AST based on the given stream (used during testing)
    void parse_only(std::istream&);

    private:

    std::shared_ptr<IRStatementList> ir_root;

    void SemanticAnalyze();

    void TranslateToIR();
    
    // Will return tru if the given node can be used as a statement
    bool StatementValid(const std::shared_ptr<Node>);
    int lexer_debug_level = 0;
    int parser_debug_level = 0;
    
    std::unique_ptr<AtmoLexer> lexer = nullptr;
    std::unique_ptr<yy::parser> parser = nullptr;

};
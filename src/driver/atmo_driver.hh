#include <istream>
#include <memory>
#include <iostream>
#include <utility>
#include <string.h>

// Needed for CPU architecture detection
#include <sys/utsname.h>

#include "../lexer/AtmoLexer.hh"
#include "../parser/parser.tab.hh"

#include "../ir/statements/ir_statement_list.hh"
#include "../ir/reserved_ir_registers.hh"

#include "../ir/ir_normalizer.hh"

#include "../frame/x86_frame.hh"
class AtmoDriver
{

    private:
    std::vector<std::shared_ptr<VariableSymbol>> GetGlobalVariables(std::vector<std::shared_ptr<Node>>&);
    
    std::shared_ptr<BoolList> GetWetherGlobalVariablesEscape(std::vector<std::shared_ptr<VariableSymbol>>& );

    std::ifstream inputFile;

    // Error class needs to preread the source file, so it can show error messages, but that will read the inputFile to EOF, so we need to open it seperately inside Error, that's why we need the file's path
    std::string openedFilePath;

    void ProcessArguments(std::vector<std::string>&);

    void ProcessBehaviouralFlag(const std::string&);
    void OpenFile(const std::string&);

    std::shared_ptr<IRStatement> ConvertStatementListToSequence(std::shared_ptr<IRStatementList> list);

    void SetFrameType();
    
    public:
    /// @brief Will point to the type of the frame during this compilation.
    static std::unique_ptr<Frame> globalFrameType;

    bool printIRTree = false;

    bool printCanonicalIRTree = false;
    /// @brief Will initialize the object and process the  arguments.
    /// @param params A vector containing all the received arguments.
    AtmoDriver(std::vector<std::string>& params);

    ~AtmoDriver() = default;
    void StartCompilation();
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
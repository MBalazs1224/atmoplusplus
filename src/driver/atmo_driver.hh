#include <istream>
#include <memory>
#include <iostream>
#include <utility>
#include <string.h>
#include <filesystem>

// Needed for CPU architecture detection
#include <sys/utsname.h>

#include "../lexer/AtmoLexer.hh"
#include "../parser/parser.tab.hh"

#include "../ir/statements/ir_statement_list.hh"
#include "../ir/reserved_ir_registers.hh"

#include "../ir/ir_normalizer.hh"
#include "../ir/traces/ir_trace.hh"

#include "../frame/global_frame.hh"

#include "../codegen/x86_codegen.hh"

#include "../codegen/default_temp_map.hh"

#include "../codegen/reg_allocator/linear_scan.hh"

#include "../codegen/global_strings.hh"

#include "../ir/canonical/ir_canonical.hh"


#include "vtable_info.hh"

class AtmoDriver
{

    private:
    std::vector<std::shared_ptr<VariableSymbol>> GetGlobalVariables(std::vector<std::shared_ptr<Node>>&);
    
    std::shared_ptr<BoolList> GetWetherGlobalVariablesEscape(std::vector<std::shared_ptr<VariableSymbol>>& );

    std::ifstream inputFile;

    // Error class needs to preread the source file, so it can show error messages, but that will read the inputFile to EOF, so we need to open it seperately inside Error, that's why we need the file's path
    std::string openedFilePath;

    void ProcessArguments(std::vector<std::string>&);

    void ProcessBehaviouralFlag(const std::string&, const int);
    void OpenFile(const std::string&);

    /// @brief Converts a string into a full NASM literal.
    /// @param labelName The name of the label.
    /// @param stringLiteral The string literal.
    /// @return A full NASM literal. ("LABELNAME: db CONVERTED_STRING_LITERAL")
    std::string StringToFullNasmLiteral(std::string labelName, std::string stringLiteral);

    std::shared_ptr<IRStatement> ConvertStatementListToSequence(std::shared_ptr<IRStatementList> list);

    void SetFrameType();
    
    public:
    /// @brief Will point to the type of the frame during this compilation.
    static std::unique_ptr<Frame> globalFrameType;

    std::string outputPath = "atmo_output";

    bool printIRTree = false;

    bool printCanonicalIRTree = false;

    bool printIRTraces = false;

    bool printASM = false;

    bool useDefaultTempMap = false;

    bool stopAtASM = false;


    bool shouldLink = true;
    
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
    
    // Will only build the AST based on the given stream (used during testing)
    void parse_only(std::istream&);


    private:

    std::shared_ptr<IRTraceSchedule> irTrace;

    void PrintGlobalVtables(std::ofstream&);
    void PrintGlobalStrings(std::ofstream&);

    void SemanticAnalyze();

    void TranslateToIR();

    void GenerateAssembly();

    std::shared_ptr<IRStatementList> BuildIRList(const std::vector<std::shared_ptr<Node>>& nodes);

    /// @param nodes The vector of all nodes.
    /// @return A vector only containing definition nodes.
    std::vector<std::shared_ptr<Node>> GetDefinitionNodes(std::vector<std::shared_ptr<Node>>& nodes);

    /// @brief 
    /// @param nodes The vector of all nodes.
    /// @return A vector only containing statement nodes.
    std::vector<std::shared_ptr<Node>> GetStatementNodes(std::vector<std::shared_ptr<Node>>& nodes);
    std::shared_ptr<IRStatementList> TranslateToIRTree(std::vector<std::shared_ptr<Node>>& nodes, std::vector<std::shared_ptr<VariableSymbol>>& globalVars);
    
    // Will return tru if the given node can be used as a statement
    bool StatementValid(const std::shared_ptr<Node>);
    int lexer_debug_level = 0;
    int parser_debug_level = 0;
    
    std::unique_ptr<AtmoLexer> lexer = nullptr;
    std::unique_ptr<yy::parser> parser = nullptr;

};
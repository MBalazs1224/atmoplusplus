#include "atmo_driver.hh"

std::unique_ptr<Frame> AtmoDriver::globalFrameType;

AtmoDriver::AtmoDriver(std::vector<std::string> &params)
{
    if (params.empty())
    {
        Error::ShowCompilerError("No parameters received!");
    }

    ProcessArguments(params);
}

void AtmoDriver::ProcessBehaviouralFlag(const std::string &param, const int index)
{
    if (param == "--debug-lexer")
    {
        set_lexer_debug_level(1);
    }
    else if (param == "--debug-parser")
    {
        set_parser_debug_level(1);
    }
    else if (param == "--print-ir-tree")
    {
        printIRTree = true;
    }
    else if (param == "--print-canonical-ir-tree")
    {
        printCanonicalIRTree = true;
    }
    else if (param == "--print-ir-trace")
    {
        printIRTraces = true;
    }
    else if (param == "--print-asm")
    {
        printASM = true;
    }
    else if (param == "--use-default-map")
    {
        useDefaultTempMap = true;
    }
    else if (param == "--asm")
    {
        stopAtASM = true;
    }
    else if (param == "--object")
    {
        shouldLink = false;
    }
    // The param starts with "--output-path"
    else if (param.rfind("--output-path", 0) == 0)
    {
        size_t pos = param.find('=');

        if (pos != std::string::npos)
        {
            std::string opath = param.substr(pos + 1);

            // Create the directory structure if needed

            std::filesystem::path filePath(opath);
            std::filesystem::path parentDir = filePath.parent_path();

        // Only create directories if there's a parent path (e.g., test/ in test/output.txt)
        if (!parentDir.empty()) {
            std::error_code ec;
            if (!std::filesystem::exists(parentDir)) {
                if (!std::filesystem::create_directories(parentDir, ec)) {
                    std::cerr << "Error creating directories: " << ec.message() << std::endl;
                    return;
                }
            } else if (!std::filesystem::is_directory(parentDir)) {
                std::cerr << "Parent path exists but is not a directory." << std::endl;
                return;
            }
        }

            this->outputPath = opath;
        }
        else
        {
            Error::ShowCompilerError("No '=' found in the '--output-path' parameter.");
        }
    }
    else
    {
        Error::ShowCompilerError(Helper::FormatString("Unknown flag '%s'!", param.c_str()));
    }
}

void AtmoDriver::OpenFile(const std::string &fileName)
{
    if (!openedFilePath.empty())
    {
        Error::ShowCompilerError("Input file specified multiple times!");
    }

    inputFile.open(fileName);

    if (!inputFile.is_open())
    {
        Error::ShowCompilerError(Helper::FormatString("Couldn't open file '%s'!", fileName.c_str()));
    }

    openedFilePath = fileName;
}

void AtmoDriver::ProcessArguments(std::vector<std::string> &params)
{
    for (size_t i = 0; i < params.size(); i++)
    {
        auto param = params[i];

       // All behavoural flags will start with "--"
       if (param.rfind("--", 0) == 0)
       {
            // Pass the current index as well
           ProcessBehaviouralFlag(param, i);
       }
       // If the flag didn't start with "--", it must be an input file name
       else
       {
           OpenFile(param);
       }
    }
}

void AtmoDriver::SetFrameType()
{
    struct utsname osInfo{};

    uname(&osInfo);

    // osInfo.machine will contain the cpu architecture

    if (strcmp(osInfo.machine, "x86_64") == 0)
    {
        GlobalFrame::globalFrameType = std::make_unique<x86Frame>();
    }
    else
    {
        Error::ShowCompilerError(Helper::FormatString("The current architecture (%s) is not supported!", osInfo.machine));
        exit(EXIT_FAILURE);
    }
}

void AtmoDriver::StartCompilation()
{
    if (openedFilePath.empty())
    {
        Error::ShowCompilerError("No input file specified!");
    }

    SetFrameType();

    lexer = CreateLexer(inputFile);

    try
    {
        parser = std::make_unique<yy::parser>(*lexer, *this);
        parser->set_debug_level(parser_debug_level);
    }
    catch (const std::bad_alloc &ba)
    {
        std::cerr << "Failed to allocate parser: (" << ba.what() << ")" << std::endl;
        exit(EXIT_FAILURE);
    }

    Error::Initialize(openedFilePath);
    SymbolTable::Initialize();
    parser->parse();
    if (!Error::CanContinue())
    {
        std::cerr << "Parsing failed, exiting!" << std::endl;
        exit(EXIT_FAILURE);
    }

    // ClassSymbol's Check funtion might need these pointers (becaues of polphormic function being offseted from RDI), so we need to initialize them here

    // Initialize the IR register pointers
    ReservedIrRegisters::Initialize();

    SemanticAnalyze();

    if (!Error::CanContinue())
    {
        return;
    }

    TranslateToIR();

    GenerateAssembly();

    if (stopAtASM)
    {
        exit(0);
    }

    system(Helper::FormatString("nasm -f elf64 -g -F dwarf %s.asm", outputPath.c_str()).c_str());

    if (!shouldLink)
    {
        exit(0);
    }

    //FIXME: No PIE executable is set, which is a security risk for modern systems
    system(Helper::FormatString("gcc -no-pie -o %s %s.o atmo_gc.o -z noexecstack", outputPath.c_str(), outputPath.c_str()).c_str());
}

std::unique_ptr<AtmoLexer> AtmoDriver::CreateLexer(std::istream &stream)
{
    try
    {
        std::unique_ptr<AtmoLexer> lex = std::make_unique<AtmoLexer>(&stream);
        lex->set_debug(lexer_debug_level);
        return lex;
    }
    catch (const std::bad_alloc &ba)
    {
        std::cerr << "Failed to allocate scanner: (" << ba.what() << ")" << std::endl;
        exit(EXIT_FAILURE);
    }
    return nullptr;
}

std::string AtmoDriver::StringToFullNasmLiteral(std::string labelName, std::string stringLiteral)
{
    std::stringstream output;
    output << labelName << ": db ";

    for (size_t i = 0; i < stringLiteral.length(); ++i) {
        unsigned char c = stringLiteral[i];

        if (c == '\\') {
            ++i;
            if (i >= stringLiteral.length()) break;

            switch (stringLiteral[i]) {
                case 'n': output << "10, "; break;
                case 't': output << "9, "; break;
                case 'r': output << "13, "; break;
                case '0': output << "0, "; break;
                case '\\': output << "92, "; break;
                case '\'': output << "39, "; break;
                case '\"': output << "34, "; break;
                default:
                    output << static_cast<int>(stringLiteral[i]) << ", ";
                    break;
            }
        } else if (isprint(c) && c != '"') {
            output << "'" << c << "', ";
        } else if (c == '"') {
            output << "34, ";
        } else {
            output << static_cast<int>(c) << ", ";
        }
    }

    output << "0"; // null terminator
    return output.str();
}

std::vector<std::shared_ptr<VariableSymbol>> AtmoDriver::GetGlobalVariables(std::vector<std::shared_ptr<Node>> &nodes)
{
    std::vector<std::shared_ptr<VariableSymbol>> globalVariables;

    for (auto &&node : nodes)
    {
        // If the variable was defined globally
        if (auto varDef = std::dynamic_pointer_cast<VariableDefinitionNode>(node))
        {
            globalVariables.push_back(varDef->GetVariable());
        }
        // If the variable was defined inside a nested scope
        else if (auto varContainer = std::dynamic_pointer_cast<BodyContainer>(node))
        {
            auto variables = varContainer->GetVariables();

            // Insert the variables into the end of the global vars
            globalVariables.insert(globalVariables.end(), variables.begin(), variables.end());
        }
    }

    return globalVariables;
}

std::shared_ptr<BoolList> AtmoDriver::GetWetherGlobalVariablesEscape(std::vector<std::shared_ptr<VariableSymbol>> &variables)
{
    // Global variables should be in the frame, so true will indicate that
    std::shared_ptr<BoolList> head = nullptr;
    std::shared_ptr<BoolList>* tail = &head;

    for (const auto& var : variables)
    {
        auto node = std::make_shared<BoolList>(true, nullptr);
        node->sizeOfVariable = var->GetSize();
        *tail = node;
        tail = &((*tail)->tail);
    }

    return head;
}

std::shared_ptr<IRStatement> AtmoDriver::ConvertStatementListToSequence(std::shared_ptr<IRStatementList> list)
{
    // If there are no statements return null
    if (!list)
        return nullptr;
    // If only one statement,return that one
    if (!list->tail)
        return list->head;

    return std::make_shared<IRSequence>(
        list->head,
        ConvertStatementListToSequence(list->tail));
}

std::vector<std::shared_ptr<Node>> AtmoDriver::GetDefinitionNodes(std::vector<std::shared_ptr<Node>> &nodes)
{
    std::vector<std::shared_ptr<Node>> definitions;

    for (auto &&node : nodes)
    {
        // If the node is a function or class definitions, push it to the def vector
        if (std::dynamic_pointer_cast<FunctionDefinitionNode>(node) || std::dynamic_pointer_cast<ClassDefinitionNode>(node))
        {
            definitions.push_back(node);
        }
    }

    return definitions;
}

std::vector<std::shared_ptr<Node>> AtmoDriver::GetStatementNodes(std::vector<std::shared_ptr<Node>> &nodes)
{
    std::vector<std::shared_ptr<Node>> statements;

    for (auto &&node : nodes)
    {
        // If the node is not a function or class definitions, push it to the stmt vector
        if (!std::dynamic_pointer_cast<FunctionDefinitionNode>(node) && !std::dynamic_pointer_cast<ClassDefinitionNode>(node))
        {
            statements.push_back(node);
        }
    }

    return statements;
}

std::shared_ptr<IRStatementList> AtmoDriver::BuildIRList(const std::vector<std::shared_ptr<Node>> &nodes)
{
    std::shared_ptr<IRStatementList> root = nullptr;
    std::shared_ptr<IRStatementList> tail = nullptr;

    for (const auto &node : nodes)
    {
        if (auto ir = node->TranslateToIR())
        {
            auto ir_list = std::make_shared<IRStatementList>(ir, nullptr);
            if (!root)
            {
                root = tail = ir_list;
            }
            else
            {
                tail->tail = ir_list;
                tail = ir_list;
            }
        }
    }
    return root;
}

std::shared_ptr<IRStatementList> AtmoDriver::TranslateToIRTree(std::vector<std::shared_ptr<Node>> &nodes, std::vector<std::shared_ptr<VariableSymbol>> &globalVars)
{

    std::vector<std::shared_ptr<Node>> definitions = GetDefinitionNodes(nodes);
    std::vector<std::shared_ptr<Node>> statements = GetStatementNodes(nodes);

    std::shared_ptr<IRStatementList> irRoot = nullptr;

    auto definitionIR = BuildIRList(definitions);
    auto statementsIR = BuildIRList(statements);

    // Automatically return 0 when reaching the end of the statemets

    auto endOfStatements = statementsIR;

    while (endOfStatements->tail)
    {
        endOfStatements = endOfStatements->tail;
    }

    auto move0IntoRax = std::make_shared<IRMove>(
        GlobalFrame::globalFrameType->ReturnLocation(),
        std::make_shared<IRConst>(0));

    auto leave = std::make_shared<IRLeave>();

    auto ret = std::make_shared<IRReturn>();

    auto ret0List = std::make_shared<IRStatementList>(
        move0IntoRax,
        std::make_shared<IRStatementList>(
            leave,
            std::make_shared<IRStatementList>(
                ret,
                nullptr)));

    endOfStatements->tail = ret0List;

    int sizeOfGlobalVariables = 0;

    for (auto &&var : globalVars)
    {
        sizeOfGlobalVariables += var->GetSize();
    }

    // Enter instruction, followed by the rest of the statements

    auto enterIns = std::make_shared<IRStatementList>(
        std::make_shared<IREnter>(sizeOfGlobalVariables),
        statementsIR);

    // Create a label for the main entry point, which is followed by the enter instruction
    auto mainLabel = std::make_shared<IRStatementList>(
        std::make_shared<IRLabel>(
            std::make_shared<Label>("main")),
        enterIns);

    // Connect definitions to mainLabel if definitions exist
    if (definitionIR != nullptr)
    {
        // Find the end
        auto defTail = definitionIR;
        while (defTail->tail != nullptr)
        {
            defTail = defTail->tail;
        }
        defTail->tail = mainLabel;
        irRoot = definitionIR;
    }
    else
    {
        // If no definitions, the whole thing starts with mainLabel
        irRoot = mainLabel;
    }

    return irRoot;
}

void AtmoDriver::PrintGlobalVtables(std::ofstream& asmFile)
{
    // Loop through all the vtable infos
    for (auto &&vtableInfo : GlobalVtables::vtables)
    {
        // Print the class's name
        asmFile << vtableInfo->className << ":" << "\n";

        // Print the pointer variables
        for (auto &&pointerName : vtableInfo->functionPointers)
        {
            // dq -> 64 bit pointer
            asmFile << "\t" << "dq " << pointerName << "\n\n";
        }
        
    }
    
}

void AtmoDriver::PrintGlobalStrings(std::ofstream& asmFile)
{
    // Print the data in this form: L0: .string "asd"
    for (auto &&pair : GlobalStrings::stringToLabel)
    {
        auto stringLiteral = pair.first;
        auto labelName = pair.second->ToString();
        
        asmFile << StringToFullNasmLiteral(labelName,stringLiteral) << "\n\n";
    }
}


void AtmoDriver::GenerateAssembly()
{
    // TODO: Implement to be able to generate to other asm type
    auto codeGen = std::make_shared<x86CodeGenerator>();

    auto asmList = codeGen->CodeGen(irTrace->statements);

    // Print the output into an asm files

    std::ofstream asmFile(Helper::FormatString("%s.asm", outputPath.c_str()));

    // Generate strings

    asmFile << "section .data\n\n"; // Print read only data indicator

    PrintGlobalStrings(asmFile);


    if(!GlobalVtables::IsEmpty())
    {
        PrintGlobalVtables(asmFile);
    }

    asmFile << "section .text\n\nglobal main\nextern heapAlloc\n\nextern initArray\n\nextern GCRegisterRoot\n\nextern GCUnregisterRoot\n\n";

    auto current = asmList;

    std::shared_ptr<TempMap> tempMap = nullptr;

    if (useDefaultTempMap)
    {
        tempMap = std::make_shared<DefaultTempMap>();
    }
    else
    {
        tempMap = std::make_shared<LinearScanMap>(asmList);
    }

    while (current)
    {
        // If the instruction is not a label, we should print a \t before it for readability
        if (!std::dynamic_pointer_cast<AssemblyLabel>(current->head))
            asmFile << "\t";

        std::string ins = current->head->Format(tempMap);
        asmFile << ins << "\n";
        current = current->tail;
    }

    asmFile.close();

    if (printASM)
    {
        system(Helper::FormatString("xdg-open ./%s.asm", outputPath.c_str()).c_str());
    }
}

void AtmoDriver::TranslateToIR()
{

    auto nodes = ast_root->GetStatementsRef();

    auto global_variables = GetGlobalVariables(nodes);

    x86Frame frame;

    // Create label for main

    auto mainLabel = Label("main");

    auto globalFrame = frame.newFrame(
        mainLabel,
        GetWetherGlobalVariablesEscape(global_variables));

    // Set the variable's access to the access allocated by the frame object

    auto accessInsideFrame = globalFrame->formals;

    int varCounter = 0;

    while (accessInsideFrame != nullptr)
    {
        global_variables[varCounter]->access = accessInsideFrame->head;

        varCounter++;
        accessInsideFrame = accessInsideFrame->tail;
    }

    auto irRoot = TranslateToIRTree(nodes, global_variables);

    if (printIRTree)
    {
        // Print DOT Formatinstructions to file
        std::ofstream dotFile("ir_tree.dot");
        int nodeCounter = 0;

        dotFile << "digraph IRTree {\n";
        dotFile << irRoot->ToDotFormat(nodeCounter);
        dotFile << "}\n";
        dotFile.close();

        // Translate the instructions into png

        system("dot -Tpng ir_tree.dot -o ir_tree.png");

        // Open the png

        system("xdg-open ./ir_tree.png");
    }

    auto irSeq = ConvertStatementListToSequence(irRoot);
    auto canonicalIrRoot = IRCanonical::Linearize(irSeq);

    if (printCanonicalIRTree)
    {
        // Print DOT Formatinstructions to file
        std::ofstream dotFile("ir_canonical_tree.dot");
        int nodeCounter = 0;

        dotFile << "digraph IRTree {\n";
        dotFile << canonicalIrRoot->ToDotFormat(nodeCounter);
        dotFile << "}\n";
        dotFile.close();

        // Translate the instructions into png

        system("dot -Tpng ir_canonical_tree.dot -o ir_canonical_tree.png");

        // Open the png

        system("xdg-open ./ir_canonical_tree.png");
    }

    auto blocks = std::make_shared<IRBlock>(canonicalIrRoot);

    irTrace = std::make_shared<IRTraceSchedule>(blocks);

    if (printIRTraces)
    {
        // Print DOT Formatinstructions to file
        std::ofstream dotFile("ir_traces.dot");
        int nodeCounter = 0;

        dotFile << "digraph IRTraces{\n";

        auto statement = irTrace->statements;
        while (statement)
        {
            dotFile << statement->head->ToDotFormat(nodeCounter);

            statement = statement->tail;
        }

        dotFile << canonicalIrRoot->ToDotFormat(nodeCounter);
        dotFile << "}\n";
        dotFile.close();

        // Translate the instructions into png

        system("dot -Tpng ir_traces.dot -o ir_traces.png");

        // Open the png

        system("xdg-open ./ir_traces.png");
    }
}

std::unique_ptr<yy::parser> AtmoDriver::CreateParser(const std::unique_ptr<AtmoLexer> &lexer)
{
    try
    {
        std::unique_ptr<yy::parser> parser = std::make_unique<yy::parser>(*lexer, *this);
        parser->set_debug_level(parser_debug_level);
        return parser;
    }
    catch (const std::bad_alloc &ba)
    {
        std::cerr << "Failed to allocate parser: (" << ba.what() << ")" << std::endl;
        exit(EXIT_FAILURE);
    }
    return nullptr;
}

void AtmoDriver::parse_only(std::istream &stream)
{
    auto lexer = CreateLexer(stream);
    auto parser = CreateParser(std::move(lexer));
    Error::Initialize(openedFilePath);
    SymbolTable::Initialize();
    parser->parse();
}

void AtmoDriver::SemanticAnalyze()
{
    for (auto statement : ast_root->GetStatementsRef())
    {
        if (!StatementValid(statement))
        {
            Error::ShowError("Invalid statement!", statement->location);
            continue;
        }
        statement->Check();
    }
}

bool AtmoDriver::StatementValid(const std::shared_ptr<Node> node)
{
    if (auto expression = std::dynamic_pointer_cast<IExpressionable>(node))
    {
        // Only assignment expressions and function calls can be used as a statement

        return std::dynamic_pointer_cast<AssignmentExpression>(expression) != nullptr || std::dynamic_pointer_cast<FunctionCall>(expression) != nullptr;
    }

    // The only node not legal as a statement is constructor definition

    return std::dynamic_pointer_cast<ConstructorDefinitionNode>(node) == nullptr;
}

void AtmoDriver::set_parser_debug_level(int level)
{
    parser_debug_level = level;
}

void AtmoDriver::set_lexer_debug_level(int level)
{
    lexer_debug_level = level;
}
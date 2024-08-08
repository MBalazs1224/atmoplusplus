#include "error.hh"

int Error::error_count = 0;
int Error::MAX_ERRORS = 3;
std::vector<std::string> Error::source;
void Error::ShowError(std::string message, yy::location loc)
{
    std::cerr << Red("Error:") << " on line " << loc.begin.line << ", column " << loc.begin.column << ": " << message << std::endl;
    PrintSource(loc.begin.line, loc.begin.column, loc.end.column);
    PrintUnderline(loc.begin.column, loc.end.column);
    if (++error_count == MAX_ERRORS)
    {
        std::cerr << "Maximum number of errors reached (" << MAX_ERRORS << "), exiting!" << std::endl;
        exit(EXIT_FAILURE);
    }
    
}

void Error::ShowCompilerError(std::string message)
{
    std::cerr << Red("Error:") << " " << message << std::endl;
}

void Error::PrintSource(int line, int col_start, int col_end)
{
    std::string correct_line = source[line - 1];
    std::cout << line << ". | " << correct_line << std::endl;
}
void Error::PrintUnderline(int col_start, int col_end)
{
    //Print spaces to push the underline
    for (size_t i = 0; i < col_start; i++)
    {
        std::cout << " ";
    }
    for (size_t i = col_start; i < col_end; i++)
    {
        std::cout << Red("~");
    }
    std::cout << std::endl;
    
}

std::string Error::Red(std::string text)
{
    //FIXME: Fix stringstream
    std::stringstream s;
    s << "\033[1;31m" << text << "\033[0m";
    return s.str();
}



void Error::Initialize()
{
    std::ifstream in("test.txt");
    if (!in.good())
    {
        ShowCompilerError("Unable to open input file!");
        exit(EXIT_FAILURE);
    }
    //FIXME: There must be a better way to show source in error messages, than prereading the source into a vector
    std::string line;
    while (getline(in,line))
    {
        source.push_back(line);
    }
    
}
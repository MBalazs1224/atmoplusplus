#include "error.hh"
#include <math.h>
int Error::error_count = 0;
int Error::MAX_ERRORS = 15;
std::vector<std::string> Error::source;

bool Error::InTest = false;

bool Error::ShouldShowWarnings = true;

void Error::ShowError(const std::string& message, const yy::location& loc)
{

    
    std::cerr << Red("Error:") << " on line " << loc.begin.line << ", column " << loc.begin.column << ": " << message << std::endl;
    if (InTest)
    {
        return;
    }
    
        // Print line before error
    if (loc.begin.line > 1)
    {
        PrintSource(loc.begin.line - 1);
    }
    PrintSource(loc.begin.line);
    PrintUnderline(loc.begin.column, loc.end.column,loc.begin.line);

    // Print line after the error
    if ((size_t)loc.begin.line < source.size())
    {
        PrintSource(loc.begin.line + 1);
    }
    
    if (++error_count == MAX_ERRORS)
    {
        std::cerr << "Maximum number of errors reached (" << MAX_ERRORS << "), exiting!" << std::endl;
        exit(EXIT_FAILURE);
    }
    
}

void Error::ShowWarning(const std::string& message, const yy::location& loc)
{
    if(!ShouldShowWarnings)
    {
        return;
    }
    std::cerr << Yellow("Warning:") << " on line " << loc.begin.line << ", column " << loc.begin.column << ": " << message << std::endl;

    if(InTest)
    {
        return;
    }
        // Print line before error
    if (loc.begin.line > 1)
    {
        PrintSource(loc.begin.line - 1);
    }
    PrintSource(loc.begin.line);
PrintYellowUnderline(loc.begin.column, loc.end.column,loc.begin.line);

    // Print line after the error
    if ((size_t)loc.begin.line < source.size())
    {
        PrintSource(loc.begin.line + 1);
    }
}



void Error::ShowCompilerError(const std::string& message)
{
    std::cerr << Red("Error:") << " " << message << std::endl;
    exit(1);
}

void Error::PrintSource(int line)
{
    std::string correct_line = source[line - 1];
    if(!correct_line.empty())
    {
        std::cout << line << ". | " << correct_line << std::endl;
    }
}
void Error::PrintUnderline(int col_start, int col_end, int lint_number)
{
    // Get the number of chars in the line number
    int char_length = std::floor(std::log10(lint_number));
    // + 4 is the size of  .space | space
    for (int i = 0; i < char_length + 3; i++)
    {
        std::cout << " ";
    }
    
    
    //Print spaces to push the underline
    for (int i = 0; i < col_start; i++)
    {
        std::cout << " ";
    }
    for (int i = col_start; i < col_end; i++)
    {
        std::cout << Red("\u02DC");
    }
    std::cout << std::endl;
    
}
 // FIXME: Fix multiple functions for different functions
void Error::PrintYellowUnderline(int col_start, int col_end, int lint_number)
{
    // Get the number of chars in the line number
    int char_length = std::floor(std::log10(lint_number));
    // + 4 is the size of  .space | space
    for (int i = 0; i < char_length + 3; i++)
    {
        std::cout << " ";
    }
    
    
    //Print spaces to push the underline
    for (int i = 0; i < col_start; i++)
    {
        std::cout << " ";
    }
    for (int i = col_start; i < col_end; i++)
    {
        std::cout << Yellow("\u02DC");
    }
    std::cout << std::endl;
    
}

std::string Error::Red(const std::string& text)
{
    //FIXME: Fix stringstream
    std::stringstream s;
    s << "\033[1;31m" << text << "\033[0m";
    return s.str();
}

std::string Error::Yellow(const std::string& text)
{
    //FIXME: Fix stringstream
    std::stringstream s;
    s << "\033[1;93m" << text << "\033[0m";
    return s.str();
}

void Error::Initialize(std::string& inputFilePath)
{
    std::ifstream reOpen(inputFilePath);

    //FIXME: There must be a better way to show source in error messages, than prereading the source into a vector
    std::string line;

    while (getline(reOpen,line))
    {
        source.push_back(line);
    }

    reOpen.close();
    
}

bool Error::CanContinue()
{
    return error_count == 0;
}

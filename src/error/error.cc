#include "error.hh"

int Error::error_count = 0;
int Error::MAX_ERRORS = 3;

void Error::ShowError(std::string message, yy::location loc)
{
    std::cerr << Red("Error:") << " on line " << loc.begin.line << ", column " << loc.begin.column << ": " << message << std::endl;
    if (++error_count == MAX_ERRORS)
    {
        std::cout << "Maximum number of errors reached (" << MAX_ERRORS << "), exiting!" << std::endl;
        exit(EXIT_FAILURE);
    }
    
}

std::string Error::Red(std::string text)
{
    std::stringstream s;
    s << "\033[1;31m" << text << "\033[0m";
    return s.str();
}
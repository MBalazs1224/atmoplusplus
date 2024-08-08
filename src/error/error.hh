#include <string>
#include <sstream>
#include "../../location.hh"
class Error
{
    private:
        static int MAX_ERRORS;
        static int error_count;
        static std::string Red(std::string);
    public:
        static void ShowError(std::string,yy::location);
};
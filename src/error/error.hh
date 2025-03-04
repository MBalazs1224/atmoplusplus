#pragma once
#include <string>
#include <sstream>
#include <vector>
#include "../parser/location.hh"
#include <fstream>
#include <cstdarg>
class Error
{
    private:
        static int MAX_ERRORS;
        static int error_count;
        static std::string Red(const std::string&);
        static std::string Yellow(const std::string&);
        // The vector containing the source code line-by-line, used for error messages
        static std::vector<std::string> source;


        Error()
        {

        }
        static void PrintSource(int);
        static void PrintUnderline(int,int,int);
        static void PrintYellowUnderline(int,int,int);
    public:
        static void ShowCompilerError(const std::string&);

        // During testing there is no source code, so it will segfault if it tries to print it,that's why there is this flag, that tells the functions to behave diferently inside tests
        static bool InTest;

        // Only prints warnings if this is true
        static bool ShouldShowWarnings;

        static void ShowError(const std::string&,const yy::location&);
        static void ShowWarning(const std::string&,const yy::location&);

        static void Initialize(std::string&);

        
        static bool CanContinue();

};
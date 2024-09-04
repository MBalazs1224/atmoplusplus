#pragma once
#include <string>
#include <sstream>
#include <vector>
#include "../../location.hh"
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

        };
        static void ShowCompilerError(const std::string&);
        static void PrintSource(int);
        static void PrintUnderline(int,int,int);
        static void PrintYellowUnderline(int,int,int);
    public:
        static void ShowError(const std::string&,const yy::location&);
        static void ShowWarning(const std::string&,const yy::location&);
        static void Initialize();
        // TODO: Might need to put FormatString fucntion into a different helper class
        static std::string FormatString(const char* format, ...);
        
        static bool CanContinue();

};
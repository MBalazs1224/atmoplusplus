#pragma once
#include <string>
#include <sstream>
#include <vector>
#include "../../location.hh"
#include <fstream>
class Error
{
    private:
        static int MAX_ERRORS;
        static int error_count;
        static std::string Red(const std::string&);
        // The vector containing the source code line-by-line, used for error messages
        static std::vector<std::string> source;


        Error()
        {

        };
        static void ShowCompilerError(const std::string&);
        static void PrintSource(int);
        static void PrintUnderline(int,int,int);
    public:
        static void ShowError(const std::string&,const yy::location&);
        static void Initialize();

};
#pragma once
#include <string>
#include <cstdarg>
class Helper
{
    private:
        Helper() = default;
    public:
        static std::string FormatString(const char* format, ...);
};
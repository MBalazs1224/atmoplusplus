#pragma once
#include <string>
#include <cstdarg>
#include "../ast/types/all_types.hh"
class Helper
{
    private:
        Helper() = default;
    public:

        // The types of the language, they are static so they can be accessed from anywhere, so other classes doesn't have to create new instances of the types (arrays need to be created runtime, because they contain their inner types)

        static std::shared_ptr<TypeBoolean> BooleanType;
        static std::shared_ptr<TypeChar> CharType;
        static std::shared_ptr<TypeFloat> FloatType;
        static std::shared_ptr<TypeInteger> IntegerType;
        static std::shared_ptr<TypeString> StringType;
        static std::shared_ptr<TypeVoid> VoidType;



        static std::string FormatString(const char* format, ...);
};
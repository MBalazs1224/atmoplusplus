#include "helper.hh"


std::shared_ptr<TypeBoolean> Helper::BooleanType = std::make_shared<TypeBoolean>();
std::shared_ptr<TypeChar> Helper::CharType = std::make_shared<TypeChar>();
std::shared_ptr<TypeFloat> Helper::FloatType = std::make_shared<TypeFloat>();
std::shared_ptr<TypeInteger> Helper::IntegerType = std::make_shared<TypeInteger>();
std::shared_ptr<TypeString> Helper::StringType = std::make_shared<TypeString>();
std::shared_ptr<TypeVoid> Helper::VoidType = std::make_shared<TypeVoid>();

std::string Helper::FormatString(const char* format, ...) {
    va_list args;
    va_start(args, format);

    // Determine the required size
    int size = std::vsnprintf(nullptr, 0, format, args) + 1; // +1 for null terminator
    va_end(args);

    // Fill the string with nulls

    std::string result(size, '\0');

    // Fill the string with the format

    va_start(args, format);
    std::vsnprintf(&result[0], size, format, args);
    va_end(args);

    return result;
}
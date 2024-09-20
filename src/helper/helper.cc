#include "helper.hh"

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
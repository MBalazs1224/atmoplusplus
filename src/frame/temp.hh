#pragma once
#include <string>
#include <memory>
#include "datasize.hh"
// Represents a temporary storage location (register)
class Temp
{
    private:
        static int count;
        int num;
    private:
        // Creates an exact copy of the temp with the givven size.
        Temp(int, DataSize);
    public:
        DataSize sizeNeeded;
        Temp();
        Temp(DataSize);
        ~Temp() = default;
        std::string ToString();
        
        /// @brief Will clone the temporary wit hthe neede size.
        /// @param wantedSize How much space the cloned temporary needs.
        /// @return The cloned temporary.
        std::shared_ptr<Temp> Clone(DataSize wantedSize);
};
#pragma once
#include <string>
#include "access.hh"
#include "temp.hh"
#include <memory>
// Represents a temporary storage location (register)
class InReg : public Access
{  
    private:
        std::shared_ptr<Temp> temp;
    public:
        /// @brief Will generate a new temporary.
        InReg();
        ~InReg() = default;
        
        /// @brief Will used the given temporary as the location.
        /// @param temp The wanted temporary location.
        InReg(std::shared_ptr<Temp> temp) ;

        std::string ToString();

        std::shared_ptr<IRExpression> AsExpression(std::shared_ptr<IRExpression> framePointer) override;
};
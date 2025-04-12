#pragma once
#include "ir_statement.hh"

// Will indicate the the ret instruction needs to be printed
class IRReturn : public IRStatement, public std::enable_shared_from_this<IRReturn>
{
    public:
        std::string ToDotFormat(int&) override;

                                                                
        std::shared_ptr<IRStatement> Build (std::shared_ptr<IRExpressionList> kids) override;
        std::shared_ptr<IRExpressionList> Kids () override;
};
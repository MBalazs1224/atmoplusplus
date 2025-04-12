#pragma once
#include "ir_statement.hh"

// Indicates that the system call instruction is needed
class IRSysCall : public IRStatement, public std::enable_shared_from_this<IRSysCall>
{
    public:
        std::string ToDotFormat(int&) override;

                                                                                
        std::shared_ptr<IRStatement> Build (std::shared_ptr<IRExpressionList> kids) override;
        std::shared_ptr<IRExpressionList> Kids () override;
};
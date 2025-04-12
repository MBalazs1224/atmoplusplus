#pragma once
#include "ir_statement.hh"

// Will indicate the the enter instruction needs to be printed
class IREnter : public IRStatement, public std::enable_shared_from_this<IREnter>
{
    public:
        int neededBytes;
        IREnter(int);
        std::string ToDotFormat(int&) override;

        std::shared_ptr<IRStatement> Build (std::shared_ptr<IRExpressionList> kids) override;
        std::shared_ptr<IRExpressionList> Kids () override;
};
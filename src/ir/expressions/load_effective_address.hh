#pragma once

#include "ir_expression.hh"
#include "ir_expression_list.hh"

class IRLoadEffectiveAddress : public IRExpression
{
    public:
        std::shared_ptr<IRExpression> source;
        

        IRLoadEffectiveAddress(std::shared_ptr<IRExpression>);

        ~IRLoadEffectiveAddress() override = default;

        std::string ToDotFormat(int&) override;

                                        
        std::shared_ptr<IRExpression> Build (std::shared_ptr<IRExpressionList> kids) override;
        std::shared_ptr<IRExpressionList> Kids () override;

        DataSize Size() override;
};
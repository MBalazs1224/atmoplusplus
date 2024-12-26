#pragma once

class IRExpressionList
{

    public:
        std::shared_ptr<IRExpression> expression;
        std::shared_ptr<IRExpressionList> next;

        
};
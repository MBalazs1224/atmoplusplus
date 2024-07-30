#include "expression.hh"
class LessThanExpression : public Expression
{
    public:
    LessThanExpression(std::unique_ptr<Expression> left_in, std::unique_ptr<Expression> right_in) : Expression(std::move(left_in),std::move(right_in))
    {

    }
    ~LessThanExpression() override = default;
    std::string Type() override
    {
        return "LessThan";
    }
};
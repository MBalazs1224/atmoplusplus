#include "expression.hh"
class SubtractExpression : public Expression
{
    public:
    SubtractExpression(std::unique_ptr<Expression> left_in, std::unique_ptr<Expression> right_in) : Expression(std::move(left_in),std::move(right_in))
    {

    }
    ~SubtractExpression() override = default;
    std::string Type() override
    {
        return "Subtract";
    }
};
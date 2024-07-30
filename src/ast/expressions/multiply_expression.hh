#include "expression.hh"
class MultiplyExpression : public Expression
{
    public:
    MultiplyExpression(std::unique_ptr<Expression> left_in, std::unique_ptr<Expression> right_in) : Expression(std::move(left_in),std::move(right_in))
    {

    }
    ~MultiplyExpression() override = default;
    std::string Type() override
    {
        return "Multiply";
    }
};
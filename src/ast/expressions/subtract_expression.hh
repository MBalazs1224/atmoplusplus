#include "expression.hh"
class SubtractExpression : public Expression
{
    public:
    SubtractExpression(std::unique_ptr<IExpressionable> left_in, std::unique_ptr<IExpressionable> right_in) : Expression(std::move(left_in),std::move(right_in))
    {

    }
    ~SubtractExpression() override = default;
    std::string Type() override
    {
        return "Subtract";
    }
};
#include "expression.hh"
class MultiplyExpression : public Expression
{
    public:
    MultiplyExpression(std::unique_ptr<IExpressionable> left_in, std::unique_ptr<IExpressionable> right_in) : Expression(std::move(left_in),std::move(right_in))
    {

    }
    ~MultiplyExpression() override = default;
    std::string Type() override
    {
        return "Multiply";
    }
};
#include "expression.hh"
class SubtractExpression : public Expression
{
    public:
    SubtractExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in) : Expression(std::move(left_in),std::move(right_in))
    {

    }
    ~SubtractExpression() override = default;
};
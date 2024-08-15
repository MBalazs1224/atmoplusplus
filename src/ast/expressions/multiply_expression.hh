#include "expression.hh"
class MultiplyExpression : public Expression
{
    public:
    MultiplyExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in) : Expression(std::move(left_in),std::move(right_in))
    {

    }
    ~MultiplyExpression() override = default;
};
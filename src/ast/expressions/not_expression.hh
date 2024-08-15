#include "expression.hh"
class NotExpression : public Expression
{
    public:
    NotExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in) : Expression(std::move(left_in),std::move(right_in))
    {

    }
    ~NotExpression() override = default;
};
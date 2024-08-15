#include "expression.hh"
class OrExpression : public Expression
{

    public:
    OrExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in) : Expression(std::move(left_in),std::move(right_in))
    {

    }
    ~OrExpression() override = default;
};
#include "expression.hh"
class NotMatchesExpression : public Expression
{
    public:
    NotMatchesExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc) : Expression(std::move(left_in),std::move(right_in),loc)
    {

    }
    ~NotMatchesExpression() override = default;
};
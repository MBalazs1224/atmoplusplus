#include "expression.hh"
class GreaterThanExpression : public Expression
{
  

    public:
    GreaterThanExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc) : Expression(std::move(left_in),std::move(right_in),loc)
    {

    }
    ~GreaterThanExpression() override = default;
};
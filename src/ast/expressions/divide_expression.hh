#include "expression.hh"
class DivideExpression : public Expression
{

    public:
    DivideExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in) : Expression(std::move(left_in),std::move(right_in))
    {

    }
    ~DivideExpression() override = default;
};
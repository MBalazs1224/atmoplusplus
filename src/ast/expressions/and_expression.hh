#include "expression.hh"

class AndExpression : public Expression
{


    public:
    AndExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in) : Expression(std::move(left_in),std::move(right_in))
    {

    }
    ~AndExpression() override = default;
};
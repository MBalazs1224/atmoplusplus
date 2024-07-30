#include "expression.hh"
class GreaterThanExpression : public Expression
{
  

    public:
    GreaterThanExpression(std::unique_ptr<IExpressionable> left_in, std::unique_ptr<IExpressionable> right_in) : Expression(std::move(left_in),std::move(right_in))
    {

    }
    ~GreaterThanExpression() override = default;
    std::string Type() override
    {
        return "GreaterThan";
    }
};
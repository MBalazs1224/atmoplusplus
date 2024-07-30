#include "expression.hh"
class OrExpression : public Expression
{

    public:
    OrExpression(std::unique_ptr<IExpressionable> left_in, std::unique_ptr<IExpressionable> right_in) : Expression(std::move(left_in),std::move(right_in))
    {

    }
    ~OrExpression() override = default;
    std::string Type() override
    {
        return "Or";
    }
};
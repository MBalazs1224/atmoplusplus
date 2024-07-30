#include "expression.hh"
class DivideExpression : public Expression
{

    public:
    DivideExpression(std::unique_ptr<IExpressionable> left_in, std::unique_ptr<IExpressionable> right_in) : Expression(std::move(left_in),std::move(right_in))
    {

    }
    ~DivideExpression() override = default;
    std::string Type() override
    {
        return "Divide";
    }
};
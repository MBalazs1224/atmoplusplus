#include "expression.hh"
class DivideExpression : public Expression
{

    public:
    DivideExpression(std::unique_ptr<Expression> left_in, std::unique_ptr<Expression> right_in) : Expression(std::move(left_in),std::move(right_in))
    {

    }
    ~DivideExpression() override = default;
    std::string Type() override
    {
        return "Divide";
    }
};
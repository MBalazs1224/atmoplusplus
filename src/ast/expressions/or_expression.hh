#include "expression.hh"
class OrExpression : public Expression
{

    public:
    OrExpression(std::unique_ptr<Expression> left_in, std::unique_ptr<Expression> right_in) : Expression(std::move(left_in),std::move(right_in))
    {

    }
    ~OrExpression() override = default;
    std::string Type() override
    {
        return "Or";
    }
};
#include "expression.hh"
class NotMatchesExpression : public Expression
{
    public:
    NotMatchesExpression(std::unique_ptr<Expression> left_in, std::unique_ptr<Expression> right_in) : Expression(std::move(left_in),std::move(right_in))
    {

    }
    ~NotMatchesExpression() override = default;
    std::string Type() override
    {
        return "NotMatches";
    }
};
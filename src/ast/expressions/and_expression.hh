#include "expression.hh"

class AndExpression : public Expression
{


    public:
    AndExpression(std::unique_ptr<Expression> left_in, std::unique_ptr<Expression> right_in) : Expression(std::move(left_in),std::move(right_in))
    {

    }
    ~AndExpression() override = default;
    std::string Type() override
    {
        return "And";
    }
};
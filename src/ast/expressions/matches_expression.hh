#include "expression.hh"
class MatchesExpression : public Expression
{
    public:
    MatchesExpression(std::unique_ptr<IExpressionable> left_in, std::unique_ptr<IExpressionable> right_in) : Expression(std::move(left_in),std::move(right_in))
    {

    }
    ~MatchesExpression() override = default;
    std::string Type() override
    {
        return "Matches";
    }
};
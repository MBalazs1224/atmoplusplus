#include "expression.hh"
class MatchesExpression : public Expression
{
    public:
    MatchesExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in) : Expression(std::move(left_in),std::move(right_in))
    {

    }
    ~MatchesExpression() override = default;
};
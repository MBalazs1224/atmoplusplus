#include "expressionable.hh"
#include <memory>
class AndExpression : public IExpressionable
{
	std::shared_ptr<IExpressionable> exp_left;	
	std::shared_ptr<IExpressionable> exp_right;
    public:
    AndExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc) : IExpressionable(loc), exp_left(std::move(left_in)),exp_right(std::move(right_in))
    {

    }
    ~AndExpression() override = default;
};

#include "expressionable.hh"
#include <memory>
class DivideExpression : public IExpressionable
{
	std::shared_ptr<IExpressionable> exp_left;
	std::shared_ptr<IExpressionable> exp_right;
    public:
    DivideExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc) : IExpressionable(loc), exp_left(left_in), exp_right(right_in)
    {

    }
    std::shared_ptr<Type> GetType() override
    {
        // TODO: We need to check if the type of the two expression match
        return std::make_shared<TypeFloat>();
    }
    ~DivideExpression() override = default;
};

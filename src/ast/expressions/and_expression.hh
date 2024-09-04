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

    std::shared_ptr<Type> GetType() override
    {
        // TODO: We need to check if the type of the two expression match

        //FIXME: This might not need to be pointer
        return std::make_shared<TypeBoolean>();
    }
    void Check() override
    {
        //TODO: Implement And expression checking
    }
};

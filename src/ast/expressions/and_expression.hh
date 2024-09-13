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
    bool Check() override
    {
        if(!exp_left->Check() || !exp_right->Check())
        {
            return false;
        }

        // FIXME: Fix boolean dynamic memory alloc
        auto boolean = std::make_shared<TypeBoolean>();
        auto exp_left_type = exp_left->GetType();
        auto exp_right_type = exp_right->GetType();

        if (exp_left_type->NotEquals(boolean) || exp_right_type->NotEquals(boolean))
        {
            Error::ShowError(Error::FormatString("Both of the two operands of AND (&&) expression must be of type boolean! (received '%s' and '%s')",exp_left_type->ToString().c_str(),exp_right_type->ToString().c_str()),location);
            return false;
        }
        return true;
    }
};

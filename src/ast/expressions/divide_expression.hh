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
        return std::make_shared<TypeFloat>();
    }
    ~DivideExpression() override = default;

    bool Check() override
    {
        if(!exp_left->Check() || !exp_right->Check())
        {
            return false;
        }

        auto exp_left_type = exp_left->GetType();
        auto exp_right_type = exp_right->GetType();

        if (exp_left_type->NotEquals(exp_right_type))
        {
            Error::ShowError(Error::FormatString("The two operands of DIVIDE (/) expression must be of same type! (received '%s' and '%s')",exp_left_type->ToString().c_str(),exp_right_type->ToString().c_str()),location);
            return false;
        }
        return true;
    }
};

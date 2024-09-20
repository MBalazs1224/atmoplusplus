#include "expressionable.hh"
#include <memory>
class DivideExpression : public IExpressionable
{
	std::shared_ptr<IExpressionable> exp_left;
	std::shared_ptr<IExpressionable> exp_right;
    public:
    DivideExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc);

    std::shared_ptr<Type> GetType() override;

    ~DivideExpression() override = default;

    bool Check() override;
};

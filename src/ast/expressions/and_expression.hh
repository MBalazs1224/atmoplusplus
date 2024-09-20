#include "expressionable.hh"
#include <memory>
class AndExpression : public IExpressionable
{
	std::shared_ptr<IExpressionable> exp_left;	
	std::shared_ptr<IExpressionable> exp_right;
    public:
    AndExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc);
    
    ~AndExpression() override;

    std::shared_ptr<Type> GetType() override;

    bool Check() override;
};

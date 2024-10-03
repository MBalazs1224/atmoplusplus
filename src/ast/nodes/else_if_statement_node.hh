#pragma once
#include "node.hh"
#include "../expressions/expressionable.hh"
#include "body_node.hh"
#include <memory>
class ElseIfStatementNode : public  Node
{
    std::shared_ptr<IExpressionable> expression;
    std::unique_ptr<BodyNode> body;

    public:
        ElseIfStatementNode(std::shared_ptr<IExpressionable> expression_in ,std::unique_ptr<BodyNode> body_in, yy::location loc) : Node(loc), expression(std::move(expression_in)), body(std::move(body_in))
        {

        }
        ~ElseIfStatementNode() override = default;
        bool Check() override
        {
            if(!expression->Check())
            {
                return false;
            };
            auto exp_type = expression->GetType();

            if (exp_type->NotCompatible(std::make_shared<TypeBoolean>()))
            {
                Error::ShowError(Helper::FormatString("The expression of an else-if statement must be of type boolean! (received '%s')",exp_type->ToString().c_str()),expression->location);
                return false;
            }
            if (body->isEmpty())
            {
                Error::ShowWarning("Empty body of else-if statement!",location);
            }
            
            return body->Check();

        }
};
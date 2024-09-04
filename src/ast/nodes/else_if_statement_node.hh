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
        void Check() override
        {
            expression->Check();
            auto exp_type = expression->GetType();

            if (exp_type->NotEquals(std::make_shared<TypeBoolean>()))
            {
                Error::ShowError(Error::FormatString("The expression of an else-if statement must be of type boolean! (received '%s')",exp_type->ToString().c_str()),expression->location);
            }
            if (body->isEmpty())
            {
                Error::ShowWarning("Empty body of else-if statement!",location);
            }
            
            body->Check();

        }
};
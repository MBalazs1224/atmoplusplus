#pragma once
#include "body_node.hh"
#include "node.hh"
#include "../expressions/expressionable.hh"
#include <memory>
#include <iostream>

class UntilStatementNode : public Node
{
    std::unique_ptr<BodyNode> body;
    std::shared_ptr<IExpressionable> expression;

    public:
        UntilStatementNode(std::shared_ptr<IExpressionable> expression_in,std::unique_ptr<BodyNode> body_in,yy::location loc)
        :  Node(loc) ,body(std::move(body_in)), expression(std::move(expression_in))
        {
        }
        ~UntilStatementNode() override = default;
        bool Check() override
        {
            if(!expression->Check())
            {
                return false;
            };
            auto exp_type = expression->GetType();
            if (exp_type->NotEquals(std::make_shared<TypeBoolean>()))
            {
                Error::ShowError(Error::FormatString("The expression of an until statement must be of type boolean! (received '%s')",exp_type->ToString().c_str()),expression->location);
                return false;
            }
            
            if (body->isEmpty())
            {
                Error::ShowWarning("Empty body of until statement!",location);
            }
            return body->Check();
        }
};
#pragma once
#include <memory>
#include "../nodes/node.hh"
#include "../types/all_types.hh"
#include "../../translate/all_translate_expressions.hh"
// For showing eroor messages 
#include "../../helper/helper.hh"
#include "../../error/error.hh"
class IExpressionable : public Node
{
    protected:
        IExpressionable() : Node()
        {

        }
        IExpressionable(yy::location loc) : Node(loc)
        {
        }

    public:
        virtual ~IExpressionable() = 0;
        virtual std::shared_ptr<Type> GetType() = 0;
        
        // Will return an TranslateExpression type object that will represent the type of the  expression
        virtual std::shared_ptr<TranslateExpression> TranslateExpressionToIr() = 0;
};

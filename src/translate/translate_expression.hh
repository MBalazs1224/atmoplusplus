#include "../ir/statements/ir_statement.hh"
#include "../ir/expressions/ir_expression.hh"

#include "../frame/label.hh"

#include "../ir/expressions/all_ir_expressions.hh"
#include "../ir/statements/all_ir_statements.hh"
#include <memory>

// A class to translate expressions between expression types (ex, nx ,cx)
class TranslateExpression
{
    public:

        virtual std::shared_ptr<IRExpression> ToValueExpression() = 0;

        virtual std::shared_ptr<IRStatement> ToNoValueExpression() = 0;

        virtual std::shared_ptr<IRStatement> ToConditionExpression(std::shared_ptr<Label> trueLabel, std::shared_ptr<Label> flaseLabel) = 0;
};
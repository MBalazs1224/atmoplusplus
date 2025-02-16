#pragma once
#include <memory>
#include "two_operand_expression.hh"
#include "../../ir/reserved_ir_registers.hh"
class ArraySubscriptExpression : public TwoOperandExpression
{
    private:
        std::shared_ptr<IRExpression> TranslateIndexingIntoIR(std::shared_ptr<Temp>);

        std::shared_ptr<IRStatement> TranslateBoundaryCheckIntoIR(std::shared_ptr<Label>,std::shared_ptr<Temp>);


    public:
    ArraySubscriptExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc);
    
    ~ArraySubscriptExpression() override = default;
    std::shared_ptr<Type> GetType() override;

    bool Check() override;

    std::shared_ptr<TranslateExpression> TranslateExpressionToIr() override;
    std::shared_ptr<IRStatement> TranslateToIR() override;
};
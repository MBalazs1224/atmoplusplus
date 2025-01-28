#include "assignment_expression.hh"
#include "identifier.hh"
#include "array_subscript.hh"
#include "../../symboltable/symbols/symbolvariable.hh"
bool AssignmentExpression::Check()
{
    if (!left->Check() || !right->Check())
    {
        return false;
    }

    // Only member containers and array subscripts can be on the left side of an assignment

    if(auto array = std::dynamic_pointer_cast<ArraySubscriptExpression>(left))
    {
        auto arrayType = array->GetType();
        auto valueType = right->GetType();
        if(valueType->NotCompatible(arrayType))
        {
            Error::ShowError(Helper::FormatString("Invalid type of expression on the right side of assignment (=) expression! The expression's type ('%s') must be compatible with the array's type ('%s')!",valueType->ToString().c_str(),arrayType->ToString().c_str()),this->location);
            return false;
        }
        return true;
    }

    auto member_container = std::dynamic_pointer_cast<MemberContainer>(left);

    // Only direct variables and variables that are members of a class can be on the left side of an assignment

    if (!member_container)
    {
        Error::ShowError("Only member containers and array subscripts can appear on the left side of an assignment (=) expression!",left->location);
        return false;
    }

    // Functions cannot be assigned to

    auto variable = std::dynamic_pointer_cast<VariableSymbol>(member_container->GetElement());
    if (!variable)
    {
        Error::ShowError("Only variables can appear on the left side of an assignment (=) expression!",left->location);
        return false;
    }
    
    auto variable_type = variable->GetType();
    auto right_type = right->GetType();
    if (right_type->NotCompatible(variable_type))
    {
        Error::ShowError(Helper::FormatString("Invalid type of expression on the right side of assignment (=) expression! The expression's type ('%s') must be compatible with the variable's type ('%s')!",right_type->ToString().c_str(),variable_type->ToString().c_str()),this->location);
        return false;
    }
    return true;
}

AssignmentExpression::AssignmentExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc) : TwoOperandExpression(left_in,right_in,loc)
{
}


std::shared_ptr<Type> AssignmentExpression::GetType()
{
    return left->GetType();
}

std::shared_ptr<TranslateExpression> AssignmentExpression::TranslateExpressionToIr()
{

    // The left hand side will either be a binary plus (framePointer + offset, if the var is in the frame) or an inreg which is just a register, that's why I think it needs to be a value expression

    auto leftExp = left->TranslateExpressionToIr()->ToValueExpression();

    auto rightExp = right->TranslateExpressionToIr()->ToValueExpression();

    // Move the right expression into the left one

    auto moveExp = std::make_shared<IRMove>(
        leftExp,
        rightExp
    );

    return std::make_shared<TranslateNoValueExpression>(moveExp);
}

std::shared_ptr<IRStatement> AssignmentExpression::TranslateToIR()
{
    //TODO: "Implement AssignmentExpression to ir"
    return nullptr;
}
#include "assignment_expression.hh"
#include "identifier.hh"
#include "../../symboltable/symbols/symbolvariable.hh"
bool AssignmentExpression::Check()
{
    if (!exp_left->Check() || !exp_right->Check())
    {
        return false;
    }
    auto member_container = std::dynamic_pointer_cast<MemberContainer>(exp_left);

    // Only direct variables and variables that are members of a class can be on the left side of an assignment

    if (!member_container)
    {
        Error::ShowError("Only member containers can appear on the left side of an assignment (=) expression!",exp_left->location);
        return false;
    }

    // Functions cannot be assigned to

    auto variable = std::dynamic_pointer_cast<VariableSymbol>(member_container->GetElement());
    if (!variable)
    {
        Error::ShowError("Only variables can appear on the left side of an assignment (=) expression!",exp_left->location);
        return false;
    }
    
    auto variable_type = exp_left->GetType();
    auto exp_right_type = exp_right->GetType();
    if (variable_type->NotEquals(exp_right_type))
    {
        // TODO: Implement polyporphism so a value can be assigned to a variable of a base class type
        Error::ShowError(Helper::FormatString("Invalid type of expression on the right side of assignment (=) expression! The expression's type ('%s') must match the variable's type ('%s')!",exp_right_type->ToString().c_str(),variable_type->ToString().c_str()),exp_right->location);
        return false;
    }
    return true;
}

AssignmentExpression::AssignmentExpression(std::shared_ptr<IExpressionable> exp_left_in, std::shared_ptr<IExpressionable> exp_right_in,yy::location loc) : IExpressionable(loc), exp_left(exp_left_in),exp_right(exp_right_in)
{
}


std::shared_ptr<Type> AssignmentExpression::GetType() 
{
    return exp_left->GetType();
}
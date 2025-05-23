#include "codegen.hh"

void CodeGenerator::EmitInstruction(std::shared_ptr<AssemblyInstruction> instruction)
{
    if(last)
    {
        last = last->tail = std::make_shared<AssemblyInstructionList>(instruction,nullptr);
    }
    else
    {
        last = ilist = std::make_shared<AssemblyInstructionList>(instruction,nullptr);
    }
    
}

std::shared_ptr<TempList> CodeGenerator::AppendTempList(std::shared_ptr<Temp> head, std::shared_ptr<TempList> tail)
{
    return std::make_shared<TempList>(
        head,
        tail
    );
}

std::shared_ptr<Temp> CodeGenerator::MunchExpression(std::shared_ptr<IRExpression> exp)
{
    if(auto casted = std::dynamic_pointer_cast<IRBinaryOperator>(exp))
    {
        return MunchBinaryOperator(casted);
    }
    else if (auto casted = std::dynamic_pointer_cast<IRCall>(exp))
    {
        return MunchFunctionCall(casted);
    }
    else if (auto casted = std::dynamic_pointer_cast<IRCall>(exp))
    {
        return MunchFunctionCall(casted);
    }
    else if (auto casted = std::dynamic_pointer_cast<IRConstFloat>(exp))
    {
        return MunchConstFloat(casted);
    }
    else if (auto casted = std::dynamic_pointer_cast<IRConst>(exp))
    {
        return MunchConstInteger(casted);
    }
    else if (auto casted = std::dynamic_pointer_cast<IRMem>(exp))
    {
        return MunchMem(casted);
    }
    else if (auto casted = std::dynamic_pointer_cast<IRName>(exp))
    {
        return MunchName(casted);
    }
    else if (auto casted = std::dynamic_pointer_cast<IRTemp>(exp))
    {
        return MunchTemp(casted);
    }
    else if (auto casted = std::dynamic_pointer_cast<IRLoadEffectiveAddress>(exp))
    {
        return MunchLoadEffectiveAddress(casted);
    }

    throw std::logic_error("Invalid experssion node during code generation!");
}

void CodeGenerator::MunchStatement(std::shared_ptr<IRStatement> stmt)
{
    if (auto casted = std::dynamic_pointer_cast<IRLabel>(stmt))
    {
        MunchLabel(casted);
    }
    else if (auto casted = std::dynamic_pointer_cast<IRCJump>(stmt))
    {
        MunchCjump(casted);
    }
    else if (auto casted = std::dynamic_pointer_cast<IREnter>(stmt))
    {
        MunchEnter(casted);
    }
    else if (auto casted = std::dynamic_pointer_cast<IREvaluateExpression>(stmt))
    {
        MunchEvaluateExpression(casted);
    }
    else if (auto casted = std::dynamic_pointer_cast<IRJump>(stmt))
    {
        MunchJump(casted);
    }
    else if (auto casted = std::dynamic_pointer_cast<IRLeave>(stmt))
    {
        MunchLeave(casted);
    }
    else if (auto casted = std::dynamic_pointer_cast<IRMove>(stmt))
    {
        MunchMove(casted);
    }
    else if (auto casted = std::dynamic_pointer_cast<IRPop>(stmt))
    {
        MunchPop(casted);
    }
    else if (auto casted = std::dynamic_pointer_cast<IRPush>(stmt))
    {
        MunchPush(casted);
    }
    else if (auto casted = std::dynamic_pointer_cast<IRReturn>(stmt))
    {
        MunchReturn(casted);
    }
    else if (auto casted = std::dynamic_pointer_cast<IRSysCall>(stmt))
    {
        MunchSysCall(casted);
    }
    else
    {
        throw std::logic_error("Invalid node type during generation!");
    }
    
}

std::string CodeGenerator::RelationalOperatorToString(RelationalOperator op)
{
    switch (op)
    {
    case RelationalOperator::Matches:
        return "je";
    case RelationalOperator::NotMatches:
        return "jne";
    case RelationalOperator::GreaterThan:
        return "jg";
    case RelationalOperator::GreaterThanOrEqual:
        return "jge";
    case RelationalOperator::LessThan:
        return "jl";
    case RelationalOperator::LessThanOrEqual:
        return "jle";
    default:
        throw std::logic_error("Invalid Relational operator!");
    }
}

std::shared_ptr<AssemblyInstructionList> CodeGenerator::CodeGen(std::shared_ptr<IRStatementList> list)
{
    ilist = last = nullptr;

    
    auto current = list;

    while (current)
    {
        MunchStatement(current->head);

        current = current->tail;
    }

    return ilist;
}
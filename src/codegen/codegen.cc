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
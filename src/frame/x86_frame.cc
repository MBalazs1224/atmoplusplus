#include "x86_frame.hh"

std::shared_ptr<Frame> x86Frame::newFrame(Label name_in, std::shared_ptr<BoolList> formals_in)
{
    auto frame = std::make_shared<x86Frame>();
    frame->name = name_in;

    // Iterate through the formals and allocate a new local for each one
    for(auto it = formals_in; it != nullptr; it = it->tail)
    {
       auto access = allocLocal(it->head);
       frame->formals = std::make_shared<AccessList>(access, frame->formals);
    }

    return frame;
}


std::shared_ptr<Access> x86Frame::allocLocal(bool escapes)
{
    //TODO: Implement that not all variables are the same size
    if(escapes)
    {
        auto ret = std::make_shared<InFrame>(allocated);

        // Allocate bytes for the next variable (4 bytes is the word size)
        allocated -= 8;
        return ret;
    }
    else
    {
        return std::make_shared<InReg>();
    }
}

std::shared_ptr<Temp> x86Frame::FramePointer()
{
    // rsp acts as the frame pointer on 64bit machines

    // FIXME: This is a temporary solution, we should have a way to get the frame pointer (rsp)
    return std::make_shared<Temp>();
}

std::shared_ptr<Temp> x86Frame::ReturnLocation()
{
    // RAX is used for return value

    //FIXME: RAX needs to be returned as the return value
    return std::make_shared<Temp>();
}

std::shared_ptr<IRStatement> x86Frame::ProcessFunctionEntryAndExit1(std::shared_ptr<IRStatement> body)
 {
    // TODO: Implement ProcessFunctionEntryAndExit1 (book [~251])
    return nullptr;
 }

std::shared_ptr<IRStatement> x86Frame::ProcessFunctionEntryAndExit3(std::string functionName, std::shared_ptr<IRStatement> body)
 {

    std::vector<std::shared_ptr<IRStatement>> statements;

    // Generate label for function name.

    auto label = std::make_shared<Label>(functionName);
    auto labelInstruction = std::make_shared<IRLabel>(label);

    statements.push_back(labelInstruction);

    // Adjust stack pointer

        // Push rbo to stack

    
        // The frame size is already calculated

    auto frameSize = -allocated;

        // Generate isntruction to subtract the frameSize from the framePointer
    auto subInstruction = std::make_shared<IRBinaryOperator>(
        BinaryOperator::MINUS,
        std::make_shared<IRTemp>(this->FramePointer()),
        std::make_shared<IRConst>(frameSize)
    );

    auto evaluateSub = std::make_shared<IREvaluateExpression>(
        std::move(subInstruction)
    );

    statements.push_back(evaluateSub);

    // Add body instructions to the statements

    statements.push_back(body);


    // Reset the stack pointer

    auto addInstruction = std::make_shared<IRBinaryOperator>(
        BinaryOperator::PLUS,
        std::make_shared<IRTemp>(this->FramePointer()),
        std::make_shared<IRConst>(frameSize)
    );

    auto evaluateAdd = std::make_shared<IREvaluateExpression>(
        std::move(addInstruction)
    );

    statements.push_back(evaluateAdd);

    // TODO: Generate return instruction


    return std::make_shared<IRSequence>(statements);
 }
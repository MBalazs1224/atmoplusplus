#include "x86_frame.hh"

std::shared_ptr<Frame> x86Frame::newFrame(Label name_in, std::shared_ptr<BoolList> formals_in)
{
    auto frame = std::make_shared<x86Frame>();
    frame->name = name_in;

    // Iterate through the formals and allocate a new local for each one, the allocated list will be backwards (the last allocated will be the first pointer)
    for (auto it = formals_in; it != nullptr; it = it->tail)
    {
        auto access = allocLocal(it->head);
        frame->formals = std::make_shared<AccessList>(access, frame->formals);
    }

    return frame;
}

std::shared_ptr<Access> x86Frame::allocLocal(bool escapes)
{
    // TODO: Implement that not all variables are the same size
    if (escapes)
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

std::shared_ptr<Temp> x86Frame::StackPointer()
{
    // rsp acts as the frame pointer on 64bit machines

    return ReservedIrRegisters::StackPointer;
}

std::shared_ptr<Temp> x86Frame::ReturnLocation()
{
    // RAX is used for return value

    return ReservedIrRegisters::RAX;
}

std::shared_ptr<IRStatement> x86Frame::ProcessFunctionEntryAndExit1(std::shared_ptr<IRStatement> body)
{
    // TODO: Implement ProcessFunctionEntryAndExit1 (book [~251])
    return nullptr;
}

std::shared_ptr<IRStatement> x86Frame::ProcessFunctionEntryAndExit3(std::string functionName, std::shared_ptr<IRStatement> body)
{

    std::vector<std::shared_ptr<IRStatement>> statements;

    // ------------------------ Generate label for function name.

    auto label = std::make_shared<Label>(functionName);
    auto labelInstruction = std::make_shared<IRLabel>(label);

    statements.push_back(labelInstruction);

    // ------------------------ Adjust stack pointer (prologue)

    // Push rbp to stack

    // rsp
    auto stackPointer = this->StackPointer();
    
    //rbp
    auto rbp = ReservedIrRegisters::FramePointer;

    auto decrementSp = std::make_shared<IRBinaryOperator>(
        BinaryOperator::MINUS,
        std::make_shared<IRTemp>(stackPointer),
        std::make_shared<IRConst>(8) // 64 bits is the word size
    );

    // Update the sp (SP = SP - 8)
    auto updateSp = std::make_shared<IRMove>(
        std::make_shared<IRTemp>(stackPointer),
        decrementSp
        );

    statements.push_back(updateSp);

    auto storeRbp = std::make_shared<IRMove>(
        std::make_shared<IRMem>(
            std::make_shared<IRTemp>(stackPointer)
            ),
        std::make_shared<IRTemp>(rbp));

    statements.push_back(storeRbp);

    auto setRbp = std::make_shared<IRMove>(
        std::make_shared<IRTemp>(rbp), 
        std::make_shared<IRTemp>(stackPointer)
        );

    statements.push_back(setRbp);

    // The frame size is already calculated

    auto frameSize = -allocated;

    // Generate isntruction to subtract the frameSize from the framePointer
    auto subInstruction = std::make_shared<IRBinaryOperator>(
        BinaryOperator::MINUS,
        std::make_shared<IRTemp>(stackPointer),
        std::make_shared<IRConst>(frameSize));

    auto evaluateSub = std::make_shared<IRMove>(
        std::make_shared<IRTemp>(stackPointer),
        subInstruction
        );

    statements.push_back(evaluateSub);

    // ------------------------ Add body instructions to the statements

    statements.push_back(body);

    // ------------------------ Reset the stack pointer (epilogue)

    auto resetTheValueOfRbp = std::make_shared<IRMove>(
        std::make_shared<IRTemp>(rbp), 
        std::make_shared<IRMem>(
            std::make_shared<IRTemp>(stackPointer)
            )
        );

    statements.push_back(resetTheValueOfRbp);

    auto moveRbpBackToRsp = std::make_shared<IRMove>(
        std::make_shared<IRTemp>(stackPointer),
        std::make_shared<IRTemp>(rbp)
    );

    statements.push_back(moveRbpBackToRsp);

    

    auto incrementStackPointer = std::make_shared<IRMove>(
        std::make_shared<IRTemp>(stackPointer),
        std::make_shared<IRBinaryOperator>(
           BinaryOperator::PLUS, 
           std::make_shared<IRTemp>(stackPointer), 
           std::make_shared<IRConst>(8)
           )
        );
    

    statements.push_back(incrementStackPointer);

    // TODO: Generate return instruction

    return std::make_shared<IRSequence>(statements);
}
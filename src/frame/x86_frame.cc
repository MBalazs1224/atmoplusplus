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

void x86Frame::AllocateRegisters(std::vector<std::shared_ptr<VariableSymbol>>& params)
{
    // The number of parameters will be indexes into the vector of possible param locations which will contain the regs in a correct order

    int integerParams = 0;
    int floatParams = 0;

    for (auto &&variable : params)
    {
        auto paramType = variable->GetType();

        // The parameter is a float

        if(paramType->Compatible(Helper::FloatType))
        {
            auto correctRegister = ReservedIrRegisters::incomingFloatParameterLocations[floatParams];

            variable->access = std::make_shared<InReg>(
                correctRegister
            );

            floatParams++;
        }
        // If the paremeter is not float type, then it must be int
        else
        {
            auto correctRegister = ReservedIrRegisters::incomingIntegerParameterLocations[integerParams];

            variable->access = std::make_shared<InReg>(
                correctRegister
            );

            integerParams++;
        }
        
    }
    
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
    // rbp
    auto framePointer = ReservedIrRegisters::FramePointer;

    // Push rbp to top of the stack
    auto pushEBP = std::make_shared<IRPush>(
        std::make_shared<IRTemp>(framePointer)
    );

    statements.push_back(pushEBP);

    // move rsp into rbp

    auto moveESPIntoEBP = std::make_shared<IRMove>(
        std::make_shared<IRTemp>(framePointer),
        std::make_shared<IRTemp>(stackPointer)
    );

    statements.push_back(moveESPIntoEBP);



    // The frame size is already calculated

    auto frameSize = -allocated;

    // Generate isntruction to subtract the frameSize from the framePointer
    auto subInstruction = std::make_shared<IRBinaryOperator>(
        BinaryOperator::MINUS,
        std::make_shared<IRTemp>(stackPointer),
        std::make_shared<IRConst>(frameSize));

    auto evaluateSub = std::make_shared<IREvaluateExpression>(
        subInstruction
        );

    statements.push_back(evaluateSub);

    // ------------------------ Add body instructions to the statements

    statements.push_back(body);

    // ------------------------ Reset the stack pointer (epilogue)

    // mobe rbp into rsp

    auto moveRBPIntoRSP = std::make_shared<IRMove>(
        std::make_shared<IRTemp>(stackPointer),
        std::make_shared<IRTemp>(framePointer)
    );
    statements.push_back(moveRBPIntoRSP);

    // Pop rbp
    auto popRBP = std::make_shared<IRPop>(
        std::make_shared<IRTemp>(framePointer)
    );

    statements.push_back(popRBP);

    

    // TODO: Generate return instruction

    return std::make_shared<IRSequence>(statements);
}
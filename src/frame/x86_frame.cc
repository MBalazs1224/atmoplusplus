#include "x86_frame.hh"

std::shared_ptr<Frame> x86Frame::newFrame(Label name_in, std::shared_ptr<BoolList> formals_in)
{
    auto frame = std::make_shared<x86Frame>();
    frame->name = name_in;

    // Iterate through the formals and allocate a new local for each one, the allocated list will be backwards (the last allocated will be the first pointer)
    for (auto it = formals_in; it != nullptr; it = it->tail)
    {
        auto access = frame->allocLocal(it->head, it->sizeOfVariable);
        frame->formals = std::make_shared<AccessList>(access, frame->formals);
    }

    return frame;
}

std::shared_ptr<Access> x86Frame::allocLocal(bool escapes, int size)
{
    // TODO: Implement that not all variables are the same size
    if (escapes)
    {
        auto ret = std::make_shared<InFrame>(allocated);

        // Allocate bytes for the next variable
        allocated -= size;
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

    //TODO: Implement that if the function is inside a class the first poitner should be the this poitner and everything should be pushed to the right

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
    std::vector<std::shared_ptr<IRStatement>> statements;
    // Save callee saved registers
    // TODO: Implement only saving registers that we don't use, might need to also allocate space for them

    for (size_t i = 0; i < ReservedIrRegisters::calleeSavedRegs.size(); i++)
    {
        auto push = std::make_shared<IRPush>(
            std::make_shared<IRTemp>(ReservedIrRegisters::calleeSavedRegs[i])
        );
        statements.push_back(push);
    }

    statements.push_back(body);

    return std::make_shared<IRSequence>(statements);
    
}

int x86Frame::AlignTo16Bytes(int size)
{
     // Find how much extra space is needed
    int remainder = size % 16;
    if (remainder == 0)
    {
        // It's already aligned
        return size;
    }
    return size + (16 - remainder);
}

std::shared_ptr<IRStatement> x86Frame::ProcessFunctionEntryAndExit3(std::string functionName, std::shared_ptr<IRStatement> body)
{

    std::vector<std::shared_ptr<IRStatement>> statements;

    // ------------------------ Generate label for function name.

    auto label = std::make_shared<Label>(functionName);
    auto labelInstruction = std::make_shared<IRLabel>(label);

    statements.push_back(labelInstruction);

    // ------------------------ Adjust stack pointer (prologue), use ENTER instruction for efficiency



    // The frame size is already calculated

    auto frameSize = -allocated;

    // For now it pushes all registers onto the stack to save them, 8 bytes is the size of a push on 64 bit
    frameSize += ReservedIrRegisters::calleeSavedRegs.size() * 8;

    // The frame needs to be aligned to 16 bytes to enable nested calls
    frameSize = AlignTo16Bytes(frameSize);

    auto enterIns = std::make_shared<IREnter>(frameSize);

    statements.push_back(enterIns);

    // ------------------------ Add body instructions to the statements

    statements.push_back(body);

    // Automatically return to the caller if the bodies end is reached

    // ------------------------ Reset the stack pointer and return to caller (epilogue), use leave and ret ins

    auto leaveIns = std::make_shared<IRLeave>();



    statements.push_back(leaveIns);

    auto retIns = std::make_shared<IRReturn>();

    statements.push_back(retIns);


    

    // TODO: Generate return instruction

    return std::make_shared<IRSequence>(statements);
}
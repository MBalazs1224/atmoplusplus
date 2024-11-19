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
    if(escapes)
    {
        auto ret = std::make_shared<InFrame>(allocated);

        // Allocate bytes for the next variable (4 bytes is the word size)
        allocated -= 4;
        return ret;
    }
    else
    {
        return std::make_shared<InReg>();
    }
}
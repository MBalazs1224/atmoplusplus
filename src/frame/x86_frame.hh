#pragma once
#include "frame.hh"

#include "inframe.hh"
#include "inreg.hh"
#include "boollist.hh"
#include "access.hh"
#include "accesslist.hh"
#include "label.hh"
#include "../ir/statements/all_ir_statements.hh"

class x86Frame : public Frame
{
    private:
        // The number of bytes allocated in the frame
        int allocated = 0;
    public:
        x86Frame() = default;
        ~x86Frame() = default;

        std::shared_ptr<Frame> newFrame(Label name_in, std::shared_ptr<BoolList> formals_in) override;

        std::shared_ptr<Access> allocLocal(bool escape) override;

        std::shared_ptr<Temp> StackPointer() override;

        std::shared_ptr<Temp> ReturnLocation() override;

        std::shared_ptr<IRStatement> ProcessFunctionEntryAndExit1(std::shared_ptr<IRStatement> body) override;
        std::shared_ptr<IRStatement> ProcessFunctionEntryAndExit3(std::string functionName, std::shared_ptr<IRStatement> body) override;
};
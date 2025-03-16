#pragma once
#include "label.hh"
#include "boollist.hh"
#include "access.hh"
#include "accesslist.hh"
#include "temp.hh"
#include "../ir/statements/ir_statement.hh"
#include "../ir/reserved_ir_registers.hh"
#include <memory>
#include "../symboltable/symbols/symbolvariable.hh"

// Represents a frame in the architecture
class Frame
{
    public:
        
        /*
        * @brief Creates a new frame
        * @param name - The label of the frame
        * @param formals - The list of booleans indicating whether the formal parameters escape the frame or not
        * @return the new Frame object
        */
        virtual std::shared_ptr<Frame> newFrame(Label, std::shared_ptr<BoolList>) = 0;

        // The label of the frame that will be the label in the asm code
        Label name;

        // The list of all the formal parameters that are inside this frame
        std::shared_ptr<AccessList> formals;

        /*
        * @brief Allocates a new local variable in the frame.
        * @param escape  Whether the variable escapes the frame or not.
        * @param size  The size of the variable.
        * @return the Access object to the variable.
        */
        virtual std::shared_ptr<Access> allocLocal(bool escape, int size) = 0;


        virtual std::shared_ptr<Temp> StackPointer() = 0;

        /*
        @brief Returns the frame's return-value location.
        @return The expression that yields lcoation of the return value.
        */
        virtual std::shared_ptr<IRExpression> ReturnLocation() = 0;
        
        /*
        @brief Will move incoming formal parameters, save and restore callee-save registers then execute the body.
        @param body The IRStatement that will execute the body.
        @return The IRStatement that will point to the start of the instructions.
        */
        virtual std::shared_ptr<IRStatement> ProcessFunctionEntryAndExit1(std::shared_ptr<IRStatement> body) = 0;

        /*
        @brief Generate a label, return instruction, adjust stack pointer, reset stack pointer.
        @param body The IRStatement that will execute the body.
        @return The IRStatement that will point to the start of the instructions.
        */
        virtual std::shared_ptr<IRStatement> ProcessFunctionEntryAndExit3(std::string functionName, std::shared_ptr<IRStatement> body) = 0;
        /// @brief Will allocate all incoming parameters their correct register as their access locations.
        /// @param params The vector containing a pointer to all incoming parameters.

        virtual void AllocateRegisters(std::vector<std::shared_ptr<VariableSymbol>>& params) = 0;


};
#pragma once
#include "label.hh"
#include "boollist.hh"
#include "access.hh"
#include "accesslist.hh"
#include <memory>

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
        * @brief Allocates a new local variable in the frame
        * @param escape - whether the variable escapes the frame or not
        * @return the Access object to the variable
        */
        virtual std::shared_ptr<Access> allocLocal(bool escape) = 0;


};
#pragma once
#include <string>

class IRNode
{
    public:

    /*
    @brief Will transorm all nodes into GraphViz DOT format for easier debugging.
    @return The DOT instructions for the current node.
    */
    virtual std::string ToDotFormat(int&) = 0;

    virtual ~IRNode() = default;
};
#pragma once
#include <memory>
#include <vector>
#include "../..//symboltable/symbols/symbolvariable.hh"

// Represents a node that can contain variables inside it
class VariableContainer
{
    public:
        virtual std::vector<std::shared_ptr<VariableSymbol>> GetVariables() = 0;
};
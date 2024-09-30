#pragma once
#include "node.hh"
#include "../../symboltable/symbols/symbolclass.hh"
#include "../attributes/attribute.hh"
#include "../types/type.hh"
#include <memory>
#include <vector>

class ClassDefinitionNode : public Node
{
    // Pointer to the created class
    std::shared_ptr<ClassSymbol> class_;
    
    public:
        ClassDefinitionNode( std::shared_ptr<ClassSymbol> func_in)
        :  class_(std::move(func_in))
        {

        }
        ~ClassDefinitionNode() override = default;
        bool Check() override
        {
            return class_->Check();
        }

        std::shared_ptr<ClassSymbol> GetClass()
        {
            return class_;
        }
};
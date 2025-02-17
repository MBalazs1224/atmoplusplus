#pragma once
#include <memory>
#include "../../frame/access.hh"

// Represents a class member (variable or function)
class ClassMember
{
    public:
        // The access object that specifies where this class member can be found
        std::shared_ptr<Access> access;

        virtual ~ClassMember() = default;
};
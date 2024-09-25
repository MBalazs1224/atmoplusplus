#pragma once
#include "type.hh"
#include <memory>
#include "../../helper/helper.hh"
class Array : public Type
{
    private:
        // The innter type of the array -> int[], it would be int
        std::shared_ptr<Type> inner_type;

    public:

        Array(std::shared_ptr<Type>);

        std::string ToString() override;

        bool Equals(const std::shared_ptr<Type>) override;

};
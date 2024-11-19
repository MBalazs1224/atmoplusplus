#pragma once
#include "type.hh"
#include <memory>
#include "../../helper/helper.hh"

class IExpressionable;

// FIXME create array of b inside c itnegers a will segfault (probably the ast is wrong)

class Array : public Type
{

    public:
        // The innter type of the array -> int[], it would be int
        std::shared_ptr<Type> inner_type;

        std::shared_ptr<IExpressionable> number_of_elements;

        Array(std::shared_ptr<Type>, std::shared_ptr<IExpressionable>);

        std::string ToString() override;

        bool Compatible(const std::shared_ptr<Type>) override;

};
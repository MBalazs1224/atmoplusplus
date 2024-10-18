#pragma once
#include "../expressions/expressionable.hh"
#include "../../location/ilocation.hh"
#include "../types/type.hh"
#include <type_traits>

/*  T_value: The value contained in the literal (int, char, etc..)
    T_type: The type of this literal (TypeBoolean, TypeString etc..)

*/

template <typename T_value, typename T_type>
class Literal : public IExpressionable
{
    public:

        T_value value;
    
        virtual ~Literal() = default;
    
    
        Literal(T_value value_in)
        {
            value = value_in;
        }
    
        bool Check() override
        {
            // FIXME: Literals shouldn't be nodes
            return true;
        }
        std::shared_ptr<Type> GetType() override
        {
            return std::make_shared<T_type>();
        } 


};
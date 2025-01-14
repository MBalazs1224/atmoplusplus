#pragma once
#include "../expressions/expressionable.hh"
#include "../../location/ilocation.hh"
#include "../types/type.hh"
#include <type_traits>

/*  T_value: The value contained in the literal (int, char, etc..)
    T_type: The type of this literal (TypeBoolean, TypeString etc..)

*/

class Literal : public IExpressionable
{
    protected:
        Literal() = default;
    public:
    
        virtual ~Literal() = default;
    
    
        
    
        bool Check() override
        {
            // FIXME: Literals shouldn't be nodes
            return true;
        }

        // FIXME: Literals should'nt have TranslateToIR methods
        std::shared_ptr<IRStatement> TranslateToIR() override;


};
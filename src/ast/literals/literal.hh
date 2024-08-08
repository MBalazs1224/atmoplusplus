#pragma once
#include "../expressions/expressionable.hh"
#include "../../location/ilocation.hh"
class Literal : public IExpressionable
{
    protected:
        Literal() = default;
    public:
        virtual ~Literal() = 0;
};
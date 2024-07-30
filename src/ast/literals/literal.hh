#pragma once
#include "../expressions/expressionable.hh"
class Literal : public IExpressionable
{
    public:
        virtual ~Literal() = 0;
};
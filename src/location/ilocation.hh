#pragma once
#include "../../location.hh"

//Location inteface for everything that needs it's location stored
class ILocation
{
    public:
        yy::location location;

        virtual ~ILocation();
};
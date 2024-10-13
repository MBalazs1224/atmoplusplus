#pragma once
#include "../parser/location.hh"

//Location inteface for everything that needs it's location stored
class ILocation
{
    public:
        yy::location location;

        ILocation(yy::location loc)
        {
            location = loc;
        }

        ILocation() {}

        virtual ~ILocation();
};
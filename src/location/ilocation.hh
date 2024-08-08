#pragma once
#include "../../location.hh"

//Location inteface for everything that needs it's location stored
class ILocation
{
    private:
        yy::location location;
    public:
        void SetLocation(yy::location);
        virtual ~ILocation();
};
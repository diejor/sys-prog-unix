#include "SUV.h"
#include "Automobile.h"
#include <string>
using namespace std;

SUV::SUV() : Automobile()
{
    passengers = 0;
}

// Constructor #2
SUV::SUV(string SUVMake, int SUVModel, int SUVMileage,
         double SUVPrice, int SUVpassengers) : Automobile(SUVMake, SUVModel, SUVMileage, SUVPrice)
{
    passengers = SUVpassengers;
}

// Accessor for passengers attribute
int SUV::getPassengers()
{
    return passengers;
}

#include "Automobile.h"
#include "Truck.h"
#include <string>
using namespace std;

Truck::Truck() : Automobile()
{
    driveType = "";
}

// Constructor #2
Truck::Truck(string truckMake, int truckModel, int truckMileage,
             double truckPrice, string truckDriveType) : Automobile(truckMake, truckModel, truckMileage, truckPrice)
{
    driveType = truckDriveType;
}

// Accessor for driveType attribute
string Truck::getDriveType()
{
    return driveType;
}


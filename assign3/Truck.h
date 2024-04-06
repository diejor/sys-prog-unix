#ifndef TRUCK_H
#define TRUCK_H
#include "Automobile.h"
#include <string>
using namespace std;

// The Truck class represents a truck.
class Truck : public Automobile
{
  private:
    string driveType;

  public:
    // Default constructor
    Truck();

    // Constructor #2
    Truck(string truckMake, int truckModel, int truckMileage,
          double truckPrice, string truckDriveType);

    // Accessor for driveType attribute
    string getDriveType();
};
#endif
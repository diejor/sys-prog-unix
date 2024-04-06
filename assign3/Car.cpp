#include "Automobile.h"
#include "Car.h"
#include <string>
using namespace std;

// The Car class represents a car.

Car::Car(): Automobile()
{
    doors = 0;
}

// Constructor #2
Car::Car(string carMake, int carModel, int carMileage,
         double carPrice, int carDoors) : Automobile(carMake, carModel, carMileage, carPrice)
{
    doors = carDoors;
}

// Accessor for doors attribute
int Car::getDoors()
{
    return doors;
}

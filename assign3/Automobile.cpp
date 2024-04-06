#include <string>
#include "Automobile.h"
using namespace std;

// Default Constructor
Automobile::Automobile()
{
      make = "";
      model = 0;
      mileage = 0;
      price = 0.0;
}

// Constructor
Automobile::Automobile(string autoMake, int autoModel, int autoMileage, double autoPrice)
{
      make = autoMake;
      model = autoModel;
      mileage = autoMileage;
      price = autoPrice;
}

// Accessors
string Automobile::getMake() const
{
      return make;
}

int Automobile::getModel() const
{
      return model;
}

int Automobile::getMileage() const
{
      return mileage;
}

double Automobile::getPrice() const
{
      return price;
}

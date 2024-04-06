#ifndef AUTOMOBILE_H
#define AUTOMOBILE_H
#include <string>

using namespace std;

// The Automobile class holds general data
// about an automobile in inventory.
class Automobile
{
private:
   string make;   // The auto's make
   int model;     // The auto's year model
   int mileage;   // The auto's mileage
   double price;  // The auto's price

public:
   // Default constructor
   Automobile();
               
   // Constructor
   Automobile(string autoMake, int autoModel, int autoMileage, double autoPrice);
     
   // Accessors
   string getMake() const;
   int getModel() const;
   int getMileage() const;
   double getPrice() const;
 
};
#endif
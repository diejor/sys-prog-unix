# This is a simple makefile that generates the executable for the Automobile class hierarchy example
# @Name: Diego Rodrigues Rodriguez
# @Date: 03/02/2024
# @Class: CS 3377.501

# variables
CC = g++
CCFLAGS = -Wall -g
OBJS = main.o Automobile.o Car.o SUV.o Truck.o
PRGM = automobiles
SUBMIT = assign3-diegorr-CS3377.zip

# default target for the makefile, PRGM is the name of the executable
$(PRGM) : $(OBJS)
	$(CC) $(OBJS) -o $(PRGM)

# targets used to compile each .o file
main.o : main.cpp Automobile.h Car.h SUV.h Truck.h
Automobile.o : Automobile.cpp Automobile.h
Car.o : Car.cpp Car.h Automobile.h
SUV.o : SUV.cpp SUV.h Automobile.h
Truck.o : Truck.cpp Truck.h Automobile.h

# compile step for each .cpp file
%.o : %.cpp
	$(CC) $(CCFLAGS) -c $<

# remove object files and executable from the directory
clean:
	rm -f $(OBJS) $(PRGM) $(SUBMIT)

# create a zip file for submission
package:
	zip $(SUBMIT) *.cpp *.h Makefile

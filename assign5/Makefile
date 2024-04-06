# Makefile for Assignment 5, creates the executables for the three programs
# TwoPipesTwoChildren, TwoPipesThreeChildren, and DynPipe. Also includes a
# clean target to remove the executables and a package target to create a zip file for submission.
# @Name: Diego Rodrigues Rodriguez
# @Date: 04/06/2024
# @Class: CS 3377.501


CC = g++
CCFLAGS = -Wall -g
OBJS = TwoPipesTwoChildren.o TwoPipesThreeChildren.o DynPipe.o
PRGMS = twopipes threepipes dynpipe 
SUBMIT = assign5-diegorr-cs3377.zip

$(PRGMS): $(OBJS)
	$(CC) $(CCFLAGS) -o threepipes TwoPipesTwoChildren.o
	$(CC) $(CCFLAGS) -o twopipes TwoPipesThreeChildren.o
	$(CC) $(CCFLAGS) -o dynpipe DynPipe.o

TwoPipesTwoChildren.o: TwoPipesTwoChildren.cpp
TwoPipesThreeChildren.o: TwoPipesThreeChildren.cpp
DynPipe.o: DynPipe.cpp

$.o: $.cpp
	$(CC) $(CCFLAGS) -c $<

clean:
	rm -f $(OBJS) $(PRGMS) $(SUBMIT)

package:
	zip $(SUBMIT) *.cpp Makefile README.txt

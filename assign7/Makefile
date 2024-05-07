# @Name: Diego Rodrigues Rodriguez
# @Date: 05/06/2024
# @Class: CS 3377.501


CC = g++
CCFLAGS = -Wall -g
OBJS = echo_s.o
PRGMS = echo_s 
SUBMIT = rodrigues-diego-assign7.zip

all: $(PRGMS)

$(PRGMS): $(OBJS)
	$(CC) $(CCFLAGS) -o echo_s echo_s.o

echo_s.o: echo_s.cpp
	$(CC) $(CCFLAGS) -c echo_s.cpp

$.o: $.cpp
	$(CC) $(CCFLAGS) -c $<

clean:
	rm -f $(OBJS) $(PRGMS) $(SUBMIT)

package:
	zip $(SUBMIT) *.cpp Makefile README.txt

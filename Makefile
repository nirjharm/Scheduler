CC=g++
CFLAGS=-std=c++11

.DEFAULT_GOAL = all

all: converter randgen scheduler

randgen: randomgen.cpp
	$(CC) $(CFLAGS) -o randgen randomgen.cpp


converter: converter.cpp
	$(CC) $(CFLAGS) -o converter converter.cpp

scheduler: scheduler.cpp
	$(CC) $(CFLAGS) -o scheduler scheduler.cpp


clean: 
	rm scheduler randgen converter

SOURCEDIR = /
SOURCES = ./main.cpp
#:= $(shell find $(SOURCEDIR) -regex ".*\.\(c\)")
EXECUTABLE = ./performMeasurement
#INCLUDE	= -I
#LDFLAGS	= -L/usr/local/lib,/usr/lib/arm-linux-gnueabihf
LDLIBS = -lwiringPi -lwiringPiDev -lpthread -lm -lpthread -lrt -ldl -lsqlite3

all: climate_measurement

climate_measurement:
	g++ $(SOURCES) -o $(EXECUTABLE) -Wall $(LDLIBS) $(LDFLAGS) $(INCLUDE)
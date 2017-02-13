SOURCEDIR = /
SOURCES = ./main.cpp ./sqlite3.o
#:= $(shell find $(SOURCEDIR) -regex ".*\.\(c\)")
EXECUTABLE = /data/performMeasurement
INCLUDE	= -I/usr/local/include
LDFLAGS	= -L/usr/local/lib,/usr/lib/arm-linux-gnueabihf
LDLIBS = -lwiringPi -lwiringPiDev -lpthread -lm -lpthread -lrt -ldl

all: climate_measurement

climate_measurement:
	g++ $(SOURCES) -o $(EXECUTABLE) -Wall $(LDLIBS) $(LDFLAGS) $(INCLUDE)
SOURCEDIR = /data/rpi-climate-monitor/src
SOURCES = /data/rpi-climate-monitor/src/main.cpp 
#:= $(shell find $(SOURCEDIR) -regex ".*\.\(c\)")
EXECUTABLE = /data/rpi-climate-monitor/performMeasurement
INCLUDE	= -I/usr/local/include,/usr/include/mysql
LDFLAGS	= -L/usr/local/lib,/usr/lib/arm-linux-gnueabihf
LDLIBS = -lwiringPi -lwiringPiDev -lpthread -lm -lmysqlclient -lpthread -lz -lrt -ldl

all: climate_measurement

climate_measurement:
	g++ $(SOURCES) -o $(EXECUTABLE) -Wall $(LDLIBS) $(LDFLAGS) $(INCLUDE) $(mysql_config --cflags) $(mysql_config --libs)
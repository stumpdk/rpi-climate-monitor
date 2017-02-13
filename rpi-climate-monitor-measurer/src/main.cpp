#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <time.h>
#include <maxdetect.h>
#include <iostream>
extern "C" {
#include </data/rpi-climate-monitor/rpi-climate-monitor-measurer/src/sqlite3.h>
}

#define BUF_SIZE 256

// LED Pin - wiringPi pin 0 is BCM_GPIO 17.
// we have to use BCM numbering when initializing with wiringPiSetupSys
// when choosing a different pin number please use the BCM numbering, also
// update the Property Pages - Build Events - Remote Post-Build Event command 
// which uses gpio export for setup for wiringPiSetupSys

bool writeLog(const char *text)
{
	//Outout for debugging purposes
	printf(text);

	FILE *f = fopen("/data/rpi-climate-monitor.log", "a");
	if (f == NULL)
	{
	    printf("Error opening file!\n");
	    return false;
	}

	/* print some text */
	fprintf(f, "%s\n", text);

	return true;
}

void databaseError(sqlite3 *con, const char SQLstring[64])
{
	char status[BUF_SIZE];
	sprintf(status, "Could not execute query: %s : %s\n", sqlite3_errmsg(con), SQLstring);
	writeLog(status);
	sqlite3_close(con);
}

sqlite3* connectToDatabase()
{	
	char* databaseName; 
	
	databaseName = getenv("DB_LOCATION");
	int rc;

	sqlite3 *con;
	
	rc = sqlite3_open(databaseName, &con); 

	if (rc){
		databaseError(con, "");
		return NULL;	
	} 

	return con;
}

bool runQuery(char SQLstring[64])
{
	sqlite3 *con = connectToDatabase();

	if(con == NULL)
	{

		return false;
	}

	if (sqlite3_exec(con, SQLstring, NULL, NULL, NULL)){
		databaseError(con, SQLstring);
		return false;
	}

	return true;
}

int main(void)
{
	static int RHT03_PIN = 7;
	int temp, rh, status = 0;
	int numOfRetries = 0;
	time_t rawtime;
	struct tm * timeinfo;
	char timeString[BUF_SIZE];
	char SQLstring[BUF_SIZE];
	char readings[BUF_SIZE];

	writeLog("started");

	wiringPiSetup();

	piHiPri(55);

	//Read temperature and humidity
	status = readRHT03(RHT03_PIN, &temp, &rh);

	//If no status is returned, 
	while ((!status || (temp / 10.0 == 0.0 && rh / 10.0 == 0.0)))
	{
		delay(200);
		status = readRHT03(RHT03_PIN, &temp, &rh);
		numOfRetries++;
	}

	if(!status)
	{
		writeLog("could not get status after 10 retries");
		return 0;
	}

	//Get current, local, formatted time
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(timeString, 64, "%x %X", timeinfo);

	//Put the data in the log file
	sprintf(readings, "Time: %s, Temperature: %5.1f, Humidity: %5.1f\n", timeString, temp / 10.0, rh / 10.0);
	writeLog(readings);

	//Save the information using MySQL
	sprintf(SQLstring, "INSERT INTO measurings (`temperature`, `humidity`) VALUES('%5.1f','%5.1f');", (temp / 10.0), (rh / 10.0));

	runQuery(SQLstring);

	return 0;
}

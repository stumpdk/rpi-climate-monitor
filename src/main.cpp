#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <time.h>
#include <maxdetect.h>
#include <mysql/mysql.h>
#include <iostream>

// LED Pin - wiringPi pin 0 is BCM_GPIO 17.
// we have to use BCM numbering when initializing with wiringPiSetupSys
// when choosing a different pin number please use the BCM numbering, also
// update the Property Pages - Build Events - Remote Post-Build Event command 
// which uses gpio export for setup for wiringPiSetupSys

void databaseError(MYSQL *con, const char SQLstring[64])
{
	fprintf(stderr, "%s : %s\n", mysql_error(con), SQLstring);
	mysql_close(con);
}

MYSQL* connectToDatabase()
{	
	const char* sqlServer;
	char* username; 
	char* password; 
	char* databaseName; 
	
	username = getenv("MYSQL_USER");
	password = getenv("MYSQL_PASSWORD");
	databaseName = getenv("MYSQL_DATABASE");
	sqlServer = "db";


	MYSQL *con = mysql_init(NULL);
	
	if (mysql_real_connect(con, sqlServer, username, password, databaseName, 0, NULL, 0) == NULL){
		databaseError(con, "");
		return NULL;	
	} 

	return con;
}

bool runQuery(char SQLstring[64])
{
	MYSQL *con = connectToDatabase();

	if(con == NULL)
	{
		return false;
	}

	if (mysql_query(con, SQLstring)){
		databaseError(con, SQLstring);
		return false;
	}

	return true;
}

bool writeLog(const char *text)
{

	//Outout for debugging purposes
	printf(text);

	FILE *f = fopen("/data/rpi-climate-monitor.log", "w");
	if (f == NULL)
	{
	    printf("Error opening file!\n");
	    return false;
	}

	/* print some text */
	fprintf(f, "%s\n", text);

	return true;
}

int main(void)
{
	static int RHT03_PIN = 7;
	int temp, rh, status = 0;
	int numOfRetries = 0;
	time_t rawtime;
	struct tm * timeinfo;
	char timeString[64];
	char statusMessage[128];
	char SQLstring[64];

	writeLog("started");

	wiringPiSetup();

	piHiPri(55);

	//Read temperature and humidity
	status = readRHT03(RHT03_PIN, &temp, &rh);

	//If no status is returned, 
	while ((!status || (temp / 10.0 == 0.0 && rh / 10.0 == 0.0)) && numOfRetries < 11)
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

	printf("Time: %s, Temperature: %5.1f, Humidity: %5.1f\n", timeString, temp / 10.0, rh / 10.0);

	//Put the data in the log file
	writeLog(timeString);

	//Save the information using MySQL
	sprintf(SQLstring, "INSERT INTO measurings (`temperature`, `humidity`) VALUES('%5.1f','%5.1f')", (temp / 10.0), (rh / 10.0));

	if(!runQuery(SQLstring)){
		writeLog("could not execute query");
	}

	return 0;
}
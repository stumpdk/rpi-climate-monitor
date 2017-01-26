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

void databaseError(MYSQL *con, char SQLstring[64])
{
	fprintf(stderr, "%s : %s\n", mysql_error(con), SQLstring);
	mysql_close(con);
}

MYSQL* connectToDatabase()
{	
	char sqlServer[3] = "db";
	char username[64] = getenv("MYSQL_USER");
	char password[64] = getenv("MYSQL_PASSWORD");
	char databaseName[64] = getenv("MYSQL_DATABASE");
	
	MYSQL *con = mysql_init(NULL);
	
	if (mysql_real_connect(con, sqlServer, username, password, databaseName, 0, NULL, 0) == NULL){
		databaseError(con, "");
		return NULL;	
	} 

	return con;
}

bool runQuery(MYSQL *con, char SQLstring[64])
{
	if (mysql_query(con, SQLstring)){
		databaseError(con, SQLstring);
		return false;
	}

	return true;
}

bool writeLog(char *text)
{
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
	time_t rawtime;
	struct tm * timeinfo;
	char TimeString[64];

	writeLog("started");

	wiringPiSetupSys();

	piHiPri(55);

	//Read temperature and humidity
	readRHT03(RHT03_PIN, &temp, &rh);

	//If no status is returned, 
	//while ((!status))
	//{
//		delay(3000);
//		status = readRHT03(RHT03_PIN, &temp, &rh);
//	}


	//Get current, local, formatted time
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(TimeString, 64, "%x %X", timeinfo);

	printf("Time: %s, Temperature: %5.1f, Humidity: %5.1f\n", TimeString, (temp), (rh));

	//Put the data in the log file
	writeLog(TimeString);

	//Save the information using MySQL
	char SQLstring[64];
	sprintf(SQLstring, "INSERT INTO TempHumid (`Temperature`, `Humidity`) VALUES('%5.1f','%5.1f')", (temp / 10.0), (rh / 10.0));

	MYSQL *con = connectToDatabase();

	if(con == NULL)
	{
		return 0;
	}

	if(!runQuery(con, SQLstring)){
		return 0;
	}

	return 0;
}
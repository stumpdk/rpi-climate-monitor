#include <stdio.h>
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

void databaseError(MYSQL *con)
{
	fprintf(stderr, "%s\n", mysql_error(con));
	mysql_close(con);
}

MYSQL* connectToDatabase()
{	
	char sqlServer[64] = "localhost";
	char username[64] = "root";
	char password[64] = "";
	char databaseName[64] = "Monitoring";
	
	MYSQL *con = mysql_init(NULL);
	
	if (mysql_real_connect(con, sqlServer, username, password, databaseName, 0, NULL, 0) == NULL){
		databaseError(con);
		return NULL;	
	} 

	return con;
}

bool runQuery(MYSQL *con, char SQLstring[64])
{
	if (mysql_query(con, SQLstring)){
		databaseError(con);
		return false;
	}

	return true;
}

int main(void)
{
	static int RHT03_PIN = 7;
	int temp, rh, status = 0;
	time_t rawtime;
	struct tm * timeinfo;
	char TimeString[64];

	//wiringPiSetupSys();

	//piHiPri(55);

	//Read temperature and humidity
	status = true;//= readRHT03(RHT03_PIN, &temp, &rh);

	//If no status is returned, 
	while ((!status))
	{
		delay(3000);
		status = readRHT03(RHT03_PIN, &temp, &rh);
	}


	//Get current, local, formatted time
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(TimeString, 64, "%x %X", timeinfo);

	printf("Time: %s, Temperature: %5.1f, Humidity: %5.1f\n", TimeString, (temp), (rh));

	//Send the information to PHP, which saves the data

	//Save the information using MySQL

	char SQLstring[64];
	sprintf(SQLstring, "INSERT INTO TempHumid (ComputerTime, Temperature, Humidity) VALUES(now(),%5.1f,%5.1f)", (temp / 10.0), (rh / 10.0));

	MYSQL *con = connectToDatabase();

	if(con == NULL)
	{
		exit(0);
	}

	if(!runQuery(con, SQLstring)){
		exit(0);
	}

	return 0;
}
#!/bin/sh
# Start MySQL
service mysql restart;
#service ssh restart;

# Create database and table
echo "create database Monitoring;" | mysql -u root;
echo "use Monitoring; create table if not exists TempHumid (ComputerTime TIMESTAMP,Temperature DECIMAL(5,1), Humidity DECIMAL(5,1))"  | mysql -u root;

# Watch mysql error logs
tail -f /var/log/mysql/error.log;
while true; do sleep 2; done;
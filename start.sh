#!/bin/sh
# Start MySQL
echo 'restarting MySQL';
service mysql restart;
#service ssh restart;

# Create database and table
echo 'creating database';
echo "create database if not exists $MYSQL_DATABASE;" | mysql -u root;
echo 'creating table';
echo "use $MYSQL_DATABASE; create table if not exists TempHumid (ComputerTime TIMESTAMP,Temperature DECIMAL(5,1), Humidity DECIMAL(5,1))"  | mysql -u root;

# Watch mysql error logs
#tail -f /var/log/mysql/error.log;
#touch /var/log/rpi-climate-monitor.log
#tail -f /var/log/mysql/error.log

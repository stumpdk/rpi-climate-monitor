create database if not exists Monitoring;
use Monitoring;
create table if not exists TempHumid (ComputerTime TIMESTAMP,Temperature DECIMAL(5,1), Humidity DECIMAL(5,1));
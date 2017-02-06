create database if not exists `measurings`;
create table if not exists `measurings`.`measurings` (`time` TIMESTAMP,`temperature` DECIMAL(5,1), `humidity` DECIMAL(5,1));
#CREATE USER 'dev'@'%' IDENTIFIED BY '123456';
GRANT ALL PRIVILEGES ON *.* TO 'dev'@'%';
FLUSH PRIVILEGES;
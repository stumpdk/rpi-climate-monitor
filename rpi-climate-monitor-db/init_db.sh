#!/bin/bash
/usr/bin/mysqld_safe &
sleep 5
mysql -u root -e "CREATE DATABASE measuring"
mysql -u root measuring < /tmp/dump.sql
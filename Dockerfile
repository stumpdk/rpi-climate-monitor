# Run Raspberry Pi image on Raspberry Pi...
FROM resin/rpi-raspbian

#
# This should be done by a separate script if user is not using Docker
#
RUN apt-get update -y
RUN export DEBIAN_FRONTEND="noninteractive" && apt-get -y install mysql-server
RUN apt-get -y install apache2 php5 mysql-client vsftpd
RUN apt-get -y install build-essential && apt-get -y install git && apt-get -y install libmysqlclient-dev
RUN git clone https://github.com/WiringPi/WiringPi.git && cd WiringPi && ./build
RUN apt-get install cron

RUN useradd rpi-climate-monitor -U
USER rpi-climate-monitor -U
RUN mkdir -p /data
RUN mkdir -p /data/mysql_data
RUN chmod +x /data

RUN sed -i "s,datadir.*=.*,datadir=/data/mysql_data,g" /etc/mysql/my.cnf

#
# Clones the rpi-climate-monitor software and builds it
#
RUN git clone https://github.com/stumpdk/rpi-climate-monitor /data/rpi-climate-monitor && cd /data/rpi-climate-monitor && git checkout code-and-server-separated && cd ./src && make

#
# set cron job that performs the measuring
#
RUN crontab /data/rpi-climate-monitor/crontab

#
# ENTRYPOINT that initializes database and table
#
ENTRYPOINT ["/bin/sh", "/data/rpi-climate-monitor/start.sh"]

VOLUME /data

# enable http
EXPOSE 80
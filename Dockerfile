# Run Raspberry Pi image on Raspberry Pi...
FROM resin-rpi-raspbian

RUN apt-get update -y
RUN export DEBIAN_FRONTEND="noninteractive" && apt-get -y install mysql-server
RUN apt-get -y install apache2 php7.0 mysql-client vsftpd

RUN apt-get -y install build-essential && apt-get -y install git && apt-get -y install libmysqlclient-dev
RUN git clone https://github.com/WiringPi/WiringPi.git && cd WiringPi && ./build

RUN git clone https://github.com/stumpdk/rpi-climate-monitor
RUN cd rpi-climate-monitor
RUN ./build

# place file somewhere logical
# add init script, that initializes database and table
#RUN cp /rpi-climate-monitor/start.sh /tmp/start.sh
#RUN cp /rpi-climate-monitor/init.sql /tmp/init.sql
RUN chmod +x /rpi-climate-monitor/start.sh
RUN chmod +x /rpi-climate-monitor/init.sql

# set cron job that performs the measuring
crontab /rpi-climate-monitor/crontab


# enable http
EXPOSE 80

ENTRYPOINT ["/bin/sh", "/tmp/start.sh"]
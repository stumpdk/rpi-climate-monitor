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

#
# Clones the rpi-climate-monitor software and builds it
#
RUN git clone https://github.com/stumpdk/rpi-climate-monitor ~/rpi-climate-monitor && cd ~/rpi-climate-monitor && git checkout code-and-server-separated && cd /src && make

#
# set cron job that performs the measuring
#
crontab ~/rpi-climate-monitor/crontab

#
# add init script, that initializes database and table
#
RUN chmod +x ~/rpi-climate-monitor/start.sh


# enable http
EXPOSE 80

ENTRYPOINT ["/bin/sh", "~/rpi-climate-monitor/start.sh"]
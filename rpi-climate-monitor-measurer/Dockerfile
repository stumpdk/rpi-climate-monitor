# Run Raspberry Pi image on Raspberry Pi...
FROM resin/rpi-raspbian

#
# This should be done by a separate script if user is not using Docker
#
RUN apt-get update -y
RUN apt-get -y install build-essential && apt-get -y install git && apt-get -y install libmysqlclient-dev
RUN git clone https://github.com/WiringPi/WiringPi.git && cd WiringPi && ./build
RUN apt-get install cron

RUN mkdir -p /data

#
# Clones the rpi-climate-monitor software and builds it
#

RUN git clone https://github.com/stumpdk/rpi-climate-monitor  /data/rpi-climate-monitor && cd /data/rpi-climate-monitor && git checkout docker-compose && cd ./src && make 

#
# set cron job that performs the measuring
#
#RUN crontab /data/rpi-climate-monitor/crontab
RUN touch /data/rpi-climate-monitor.log

ADD crontab /etc/cron.d/rpi-cron

# Give execution rights on the cron job
RUN chmod 0644 /etc/cron.d/rpi-cron

#
# ENTRYPOINT that initializes database and table
#
CMD cron && tail -f /data/rpi-climate-monitor.log
#ENTRYPOINT ["/bin/sh", "/data/rpi-climate-monitor/start.sh"]

VOLUME /data

# enable http
EXPOSE 80
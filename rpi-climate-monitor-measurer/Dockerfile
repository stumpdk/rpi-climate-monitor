#WiringPi image
FROM hypriot/rpi-gpio

RUN apt-get -y update && apt-get install -y cron

RUN mkdir -p /data

#
# Clones the rpi-climate-monitor software and builds it
#
RUN git clone https://github.com/stumpdk/rpi-climate-monitor /data/rpi-climate-monitor && cd /data/rpi-climate-monitor && git checkout docker-compose && cd ./src && make

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
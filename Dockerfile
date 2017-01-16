#FROM ubuntu:latest
# use armf ubuntu ready for raspberry pi
FROM armhf/alpine:edge
#FROM multiarch/alpine:armhf-edge

# install mysql server and libraries, apache2, php and ssh server
RUN apk update && \
	export DEBIAN_FRONTEND="noninteractive" && \
	apk add mysql

RUN apk add apache2 php5-apache2 mysql-client vsftpd
#php-mysql php-mysqli

>>>>>>> a46e3c71ed8624ead587b8e42e32d448f3ffd237
# install mysql dependencies used when compiling source using mysql libraries
RUN apk add mysql-dev

RUN /usr/bin/mysql_install_db --user=mysql

<<<<<<< HEAD
RUN git clone https://github.com/WiringPi/WiringPi.git && cd WiringPi && sed -i '/sudo=${WIRINGPI_SUDO-sudo}/c\sudo=' ./build && sed -i '/PREFIX?=/local/PREFIX?=' ./build && ./build
=======
RUN apk add openssh
RUN apk add g++ && apk add gcc && apk add make
RUN apk add git
RUN apk add sudo
RUN apk add linux-headers

#RUN git clone https://github.com/WiringPi/WiringPi.git && \
#	cd WiringPi && sed -i '/sudo=${WIRINGPI_SUDO-sudo}/c\sudo=' ./build  && \
#	sed -i '/PREFIX?=\/local/c\PREFIX?=' ./wiringPi/Makefile && \
#	sed -i '/LDCONFIG?=ldconfig/c\LDCONFIG?=ldconfig /' ./wiringPi/Makefile && \
#	./build

run apk add wiringpi
>>>>>>> a46e3c71ed8624ead587b8e42e32d448f3ffd237

# copy source code
ADD ClimateSurvelliance/src /tmp/sourcecode
RUN make -f /tmp/sourcecode/makefile
#RUN echo "Running compiled program" && /tmp/performMeasurement

# set cron job that performs the measuring
# set cron job that performs alert checks
ADD init/crontab /tmp/crontab
RUN crontab /tmp/crontab

# add init script, that initializes database and table
COPY ./init/start.sh /tmp/start.sh
COPY ./init/init.sql /tmp/init.sql
RUN chmod +x /tmp/start.sh
RUN chmod +x /tmp/init.sql

VOLUME /var/lib/mysql

# enable http access	
EXPOSE 80

ENTRYPOINT ["/bin/sh", "/tmp/start.sh"]
#ENTRYPOINT ["top", "-b"]

# build: docker build -t climate_surv .
# start: docker run -d --name climate_con -p 8000:80 -p 2222:22 climate_surv
# log in: docker exec -i -t climate_con /bin/bash

#FROM ubuntu:latest
# use armf ubuntu ready for raspberry pi
FROM armhf/ubuntu


# install mysql server and libraries, apache2, php and ssh server
RUN apt-get -y update && \
	export DEBIAN_FRONTEND="noninteractive" && apt-get -y install mysql-server
RUN apt-get -y install apache2 php7.0 mysql-client vsftpd
RUN apt-get -y install openssh-server
RUN apt-get -y install build-essential
RUN apt-get -y install git
# install mysql dependencies used when compiling source using mysql libraries
RUN apt-get -y install libmysqlclient-dev

#RUN echo "root:root" | chpasswd
#RUN sed 's/#PasswordAuthentication yes/PasswordAuthentication no/g' /etc/ssh/sshd_config > /etc/ssh/sshd_config

RUN git clone https://github.com/WiringPi/WiringPi.git && cd WiringPi && sed -i '/sudo=${WIRINGPI_SUDO-sudo}/c\sudo=' ./build && ./build

# copy source code
ADD ClimateSurvelliance/src /tmp/sourcecode
#RUN mysql_config --cflags
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

# enable http and SSH access	
EXPOSE 80
EXPOSE 22 

ENTRYPOINT ["/bin/sh", "/tmp/start.sh"]
#ENTRYPOINT ["top", "-b"]

# build: docker build -t climate_surv .
# start: docker run -d --name climate_con -p 8000:80 -p 2222:22 climate_surv
# log in: docker exec -i -t climate_con /bin/bash

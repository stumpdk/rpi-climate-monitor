# rpi-climate-monitor

rpi-climate-monitor uses Docker to easily setup a simple climate monitor for your Raspberry Pi.

## Usage with Docker

Install Docker:
```sudo curl -sSL get.docker.com | sh``` or ```sudo curl -sSLk get.docker.com | sh``` if the first command gives a certificate error (insecure)

Clone this repository:
```git clone https://github.com/stumpdk/rpi-climate-monitor```

Build the Docker image:
```cd rpi-climate-monitor && sudo docker build -t rpi-climate-monitor .```

Start the container:
```sudo docker run -d --name rpi-climate-monitor -p 8000:80 -p 2222:22 rpi-climate-monitor-container```
```sudo docker run -d --entrypoint=/bin/bash --name rpi-climate-monitor -p 8000:80 -p 2222:22 rpi-climate-monitor-container resin/rpi-raspbian:jessie```

Now the monitor measures and puts data into the database every minute.


## Usage in barebones Raspberry Pi

Not yet supported. But it should work as above, as long as the dependencies used in the Dockerfile (MySQL and wiringPi among others) are installed manually, before building the monitor executable.

The build is done by cloning this repository, entering the src dir and build the file with ```make```

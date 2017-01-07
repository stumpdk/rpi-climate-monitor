# rpi-climate-monitor

rpi-climate-monitor uses Docker to easily setup a simple climate monitor for your Raspberry Pi.

## Usage

Install Docker:
```curl -sSL get.docker.com | sh``` or ```curl -sSLk get.docker.com | sh``` if the first command gives a certificate error (insecure)

Clone this repository:
````git clone https://github.com/stumpdk/rpi-climate-monitor```


Enter directory:
```cd rpi-climate-monitor```

Build the Docker image:
````sudo docker build -t rpi-climate-monitor .```

Start the container:
```sudo docker run -d --name rpi-climate-monitor --privileged -p 8000:80 -p 2222:22 rpi-climate-monitor-container```

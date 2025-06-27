# webCam2RTSP
Webcam/USBcam stream convert as RTSP stream. This is a simple rtsp sever that streaming web-cam or usb-cam.

## Dependencies
1. [Gstreamer](https://gstreamer.freedesktop.org/)

## Qmake build

		git clone https://github.com/aks-arise1600/webCam2RTSP.git
		mkdir qbuild && cd qbuild
		qmake ..
		make
		
## Cmake build

		git clone https://github.com/aks-arise1600/webCam2RTSP.git
		mkdir cbuild && cd cbuild
		cmake ..
		make

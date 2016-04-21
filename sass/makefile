SOURCES = src/*.cpp
CFLAGS=-fPIC -g -Wall -std=c++11 `pkg-config --cflags opencv libusb-1.0`
LIBS = `pkg-config --cflags --libs opencv` -pthread 
INCLUDE = -I headers -I/usr/local/include/libfreenect -I/usr/local/include/libfreenect2 #-I/usr/include/x86_64-linux-gnu/libavcodec -I/usr/include/x86_64-linux-gnu/libavformat -I/usr/include/x86_64-linux-gnu/libavutil
FREE_LIBS = -L/usr/local/lib -lfreenect -lfreenect2 
#AVC_LIBS = -L/usr/lib/x86_64-linux-gnu -lavcodec -lavformat -lavutil -lswscale

FIFO = ActiveDroneFeed

all:
	g++ $(CFLAGS) -o main $(SOURCES) $(INCLUDE) $(LIBS) $(FREE_LIBS) #$(AVC_LIBS)
	@[ -p $(FIFO) ] || mkfifo $(FIFO)

SOURCES = src/*.cpp
CFLAGS=-fPIC -g -Wall -std=c++11 `pkg-config --cflags opencv libusb-1.0`
LIBS = `pkg-config --cflags --libs opencv` -pthread
INCLUDE = -I headers -I/usr/local/include/libfreenect -I/usr/local/include/libfreenect2
FREE_LIBS = -L/usr/local/lib -lfreenect -lfreenect2

all:
	g++ $(CFLAGS) -o main $(SOURCES) $(INCLUDE) $(LIBS) $(FREE_LIBS) 

all: scanner

CFLAGS=-fPIC -g -Wall -std=c++11 `pkg-config --cflags opencv libusb-1.0`
LIBS = `pkg-config --libs opencv libusb-1.0` -pthread
INCLUDE = -I/usr/local/include/libfreenect
FREE_LIBS = -L/usr/local/lib -lfreenect

scanner:  scanner.cpp
	$(CXX) $(INCLUDE) $(CFLAGS) $? -o $@  $(LIBS) $(FREE_LIBS)

%.o: %.cpp
	$(CXX) -c $(CFLAGS) $< -o $@

clean:
	rm -rf *.o scanner

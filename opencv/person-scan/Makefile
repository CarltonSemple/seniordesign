all: test

CFLAGS=-fPIC -g -Wall `pkg-config --cflags opencv libusb-1.0`
LIBS = `pkg-config --libs opencv libusb-1.0` -pthread
INCLUDE = -I/usr/local/include/libfreenect
FREE_LIBS = -L/usr/local/lib -lfreenect

test:  test.cpp
	$(CXX) $(INCLUDE) $(CFLAGS) $? -o $@  $(LIBS) $(FREE_LIBS)

%.o: %.cpp
	$(CXX) -c $(CFLAGS) $< -o $@

clean:
	rm -rf *.o test

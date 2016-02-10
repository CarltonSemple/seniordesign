all: test

LIBS = `pkg-config --cflags --libs opencv`

test:  test.cpp
	$(CXX) $? -o $@  $(LIBS)

clean:
	rm -rf *.o test

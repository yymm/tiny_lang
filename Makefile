CXX = g++

build: parser.cpp interp.cpp
	$(CXX) -Wall -o tiny parser.cpp interp.cpp mpc/mpc.cpp -lm

clean:
	rm tiny

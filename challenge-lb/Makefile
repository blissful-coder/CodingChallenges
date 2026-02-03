CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pthread

all: lb be

lb: lb.cpp
	$(CXX) $(CXXFLAGS) -o lb lb.cpp

be: be.cpp
	$(CXX) $(CXXFLAGS) -o be be.cpp

clean:
	rm -f lb be

.PHONY: all clean

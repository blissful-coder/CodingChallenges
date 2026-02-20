CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pthread

all: lb

lb: lb.cpp
	$(CXX) $(CXXFLAGS) -o lb lb.cpp

clean:
	rm -f lb

.PHONY: all clean

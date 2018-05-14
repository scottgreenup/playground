
MAIN=main
TEST=test

CXX = clang++
CXXFLAGS = -std=c++11
CXXDEBUG = -g3
CXXPROD = -O3

all: default

default:
	$(CXX) $(CXXFLAGS) $(CXXPROD) -o $(MAIN) args.cc main.cc

tests:
	rm -f $(TEST)
	$(CXX) $(CXXFLAGS) $(CXXDEBUG) -lgtest -o $(TEST) args.cc
	./test

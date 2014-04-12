#    File:    Makefile
#    Author:  Marvin Smith
#    Date:    4/11/2014
#    
#    Purpose:  Studying some typical algorithm problems
#

CXXFLAGS=-Wall --std=c++0x

BINARIES=bin/mergesort \
		 bin/quicksort \
		 bin/move-zeros \
		 bin/missionary-and-cannabals

all:$(BINARIES)

bin/move-zeros: misc/move-zeros.cpp
	$(CXX) $< -o $@ $(CXXFLAGS)

bin/quicksort: sorting/quicksort.cpp bin
	$(CXX) $< -o $@ $(CXXFLAGS)

bin/mergesort: sorting/mergesort.cpp bin
	$(CXX) $< -o $@ $(CXXFLAGS)

bin/missionary-and-cannabals: search/missionary-and-cannabals.cpp bin
	$(CXX) $< -o $@ $(CXXFLAGS)


bin:
	mkdir bin

clean:
	rm $(BINARIES)


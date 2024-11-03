
CFLAGS=-std=c++11

OBJECTS=build/j2k-parser.o \
		build/J2K_Parsers.o \
		build/Utilities.o

HEADERS=J2K_Parsers.hpp \
		Utilities.hpp

DIRS=build

all: $(DIRS) j2k-parser

build:
	mkdir build

j2k-parser: $(HEADERS) $(OBJECTS)
	g++ -o $@ $(OBJECTS) $(CFLAGS) $(LIBS)

build/j2k-parser.o: j2k-parser.cpp $(HEADERS)
	g++ -o $@ -c $< $(CFLAGS) 

build/J2K_Parsers.o: J2K_Parsers.cpp $(HEADERS)
	g++ -o $@ -c $< $(CFLAGS) 

build/Utilities.o: Utilities.cpp $(HEADERS)
	g++ -o $@ -c $< $(CFLAGS) 

clean:
	rm build/*.o j2k-parser


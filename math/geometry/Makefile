
SOURCES = src/Vector.cpp \
	      src/Quaternion.cpp \
		  ../../unit_test/src/*.cpp

HEADERS = src/Vector.h \
	      src/Quaternion.h \
		  ../../unit_test/src/*.0

OBJECTS = build/*.o

test:	
	g++ src/unit_test.cpp $(SOURCES) -o unit_test
	./unit_test


SOURCES = src/Vector.cpp \
	      src/Quaternion.cpp \
		  src/test/Logger.cpp \
		  src/test/Color.cpp \
		  src/test/Assert.cpp

HEADERS = src/Vector.h \
	      src/Quaternion.h \
	      src/test/Logger.h \
   	      src/test/Color.h \
		  src/test/Assert.h

OBJECTS = build/Vector.o \
	      build/Quaternion.o \
		  build/Logger.o \
		  build/Color.o \
		  build/Assert.o

test:	
	g++ src/unit_test.cpp $(SOURCES) -o unit_test
	./unit_test

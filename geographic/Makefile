TEST_SOURCES = ../unit_test/src/Assert.cpp \
				   ../unit_test/src/Logger.cpp \
					../unit_test/src/Color.cpp


#ifeq ($(shell uname),Linux)
GDAL_LIBS = -L/usr/local/lib -lgdal
GDAL_INCLUDE = -I/usr/local/include
#else
#GDAL_LIBS = -L/opt/local/lib -lgdal
#GDAL_INCLUDE = -I/opt/local/include
#endif

SOURCES = src/structures/*.cpp \
		  src/utilities/*.cpp \
		  ../opencv/*.cpp 

HEADERS = src/structures/*.h \
	  	   src/utilities/*.h \
    		../opencv/*.h 

OBJECTS = build/*.o


test: $(HEADERS)
	g++ unit_test.cpp $(TEST_SOURCES) $(SOURCES) -o unit_test `pkg-config opencv --cflags --libs` $(GDAL_INCLUDE) $(GDAL_LIBS)
	./unit_test


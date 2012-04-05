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


unit_test: $(HEADERS) $(SOURCES) unit_test.cpp
	g++ unit_test.cpp $(SOURCES) -o unit_test $(GDAL_INCLUDE) $(TEST_INCLUDE) `pkg-config opencv --cflags --libs` $(GDAL_LIBS) -g

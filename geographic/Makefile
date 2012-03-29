
TEST_DIR = ../unit_test

TEST_SOURCES = $(TEST_DIR)/src/*.cpp
TEST_HEADERS = $(TEST_DIR)/src

TEST_INCLUDE = -I$(TEST_HEADERS)

ifeq ($(shell uname),Linux)
GDAL_LIBS = -L/usr/local/lib -lgdal
GDAL_INCLUDE = -I/usr/local/include
else
GDAL_LIBS = -L/opt/local/lib -lgdal
GDAL_INCLUDE = -I/opt/local/include
endif

SOURCES = src/structures/*.cpp \
		  src/utilities/*.cpp \
		  ../opencv/*.cpp \
		  $(TEST_SOURCES)

HEADERS = src/structures/*.h \
		  src/utilities/*.h \
		  ../opencv/*.h \
		  $(TEST_HEADERS)/*.h

OBJECTS = build/*.o


unit_test: $(HEADERS) $(SOURCES) unit_test.cpp
	g++ unit_test.cpp $(SOURCES) -o unit_test $(GDAL_INCLUDE) $(TEST_INCLUDE) `pkg-config opencv --cflags --libs` $(GDAL_LIBS) -g

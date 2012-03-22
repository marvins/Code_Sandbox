
TEST_DIR = ../unit_test

TEST_SOURCES = $(TEST_DIR)/src/*.cpp
TEST_HEADERS = $(TEST_DIR)/src

TEST_INCLUDE = -I$(TEST_HEADERS)

GDAL_LIBS = -L/usr/local/lib -lgdal
GDAL_INCLUDE = -I/usr/local/include

SOURCES = src/structures/*.cpp \
		  $(TEST_SOURCES)

HEADERS = src/structures/*.h \
		  $(TEST_HEADERS)/*.h

OBJECTS = build/*.o


test: $(HEADERS)
	g++ unit_test.cpp $(SOURCES) -o unit_test $(GDAL_INCLUDE) $(TEST_INCLUDE) `pkg-config opencv --cflags --libs` $(GDAL_LIBS) -g
	
	

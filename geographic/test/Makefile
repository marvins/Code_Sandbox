.PHONY: docs

BIN=bin
BUILD=build
DATA=data
SRC =src
CORE=$(SRC)/core
MATH=$(SRC)/math
ORTHO=$(SRC)/ortho


OPENCV_LIBS    = `pkg-config opencv --libs`
OPENCV_INCLUDE = `pkg-config opencv --cflags`
OPENCV         =  $(OPENCV_INCLUDE)  $(OPENCV_LIBS)

BOOST_LIBS    = -L/usr/lib -lboost_system -lboost_filesystem
BOOST_INCLUDE = -I/usr/include 
BOOST         = $(BOOST_INCLUDE) $(BOOST_LIBS)


LIBS = $(OPENCV) $(BOOST)
INCL = $(OPENCV_INCLUDE) $(BOOST_INCLUDE)

EXEC = $(BIN)/orthoproject

FLAGS = -Wall -O3

###############################
#    List of Object Files     #
###############################
OBJECTS= $(BUILD)/orthoproject.o \
         $(BUILD)/Configuration.o \
		 $(BUILD)/Parser.o \
		 $(BUILD)/Orthorectify.o \
		 $(BUILD)/Perspective.o \
		 $(BUILD)/Quaternion.o \
		 $(BUILD)/Vector.o \
		 $(BUILD)/Geometry.o \
		 $(BUILD)/Utilities.o \
		 $(BUILD)/dem.o

HEADERS= $(CORE)/Configuration.hpp \
		 $(CORE)/Parser.hpp \
		 $(ORTHO)/Orthorectify.hpp \
		 $(ORTHO)/Perspective.hpp \
		 $(MATH)/Quaternion.hpp \
		 $(MATH)/Vector.hpp \
		 $(MATH)/Geometry.hpp \
		 $(CORE)/Utilities.hpp \
		 $(CORE)/dem.hpp

###############################
#          Executable         #
###############################
$(EXEC): $(OBJECTS) $(HEADERS)
	g++ $(OBJECTS) -o $@ $(LIBS) $(FLAGS)


###############################
#          Main Driver        #
###############################
$(BUILD)/orthoproject.o: orthoproject.cpp  $(HEADERS)
	g++ $< -c -o $@ $(INCL) $(FLAGS)


# Configuration
$(BUILD)/Configuration.o: $(CORE)/Configuration.cpp $(HEADERS)
	g++ $< -c -o $@ $(INCL) $(FLAGS)

# Config Parser
$(BUILD)/Parser.o: $(CORE)/Parser.cpp $(HEADERS)
	g++ $< -c -o $@ $(INCL) $(FLAGS)

# Orthorectification
$(BUILD)/Orthorectify.o: $(ORTHO)/Orthorectify.cpp $(HEADRES)
	g++ $< -c -o $@ $(INCL) $(FLAGS)

# Perspective Code
$(BUILD)/Perspective.o: $(ORTHO)/Perspective.cpp $(HEADERS)
	g++ $< -c -o $@ $(INCL) $(FLAGS)

# Quaternion Code
$(BUILD)/Quaternion.o: $(MATH)/Quaternion.cpp $(HEADERS)
	g++ $< -c -o $@ $(INCL) $(FLAGS)

#Vector Code
$(BUILD)/Vector.o: $(MATH)/Vector.cpp $(HEADERS)
	g++ $< -c -o $@ $(INCL) $(FLAGS)

# Geometry code
$(BUILD)/Geometry.o: $(MATH)/Geometry.cpp $(HEADERS)
	g++ $< -c -o $@ $(INCL) $(FLAGS)

# Utility Functions
$(BUILD)/Utilities.o: $(CORE)/Utilities.cpp $(HEADERS)
	g++ $< -c -o $@ $(INCL) $(FLAGS)

# DEM Functions
$(BUILD)/dem.o: $(CORE)/dem.cpp $(HEADERS)
	g++ $< -c -o $@ $(INCL) $(FLAGS)

docs:
	doxygen Doxyfile

clean: 
	rm -r $(EXEC) $(BUILD)/*

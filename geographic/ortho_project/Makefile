.PHONY: docs

BIN=bin
BUILD=build
DATA=data
SRC =src
CORE=$(SRC)/core
MATH=$(SRC)/math
ORTHO=$(SRC)/ortho

#####################
#       OpenCV      #
#####################
OPENCV_LIBS    = `pkg-config opencv --libs`
OPENCV_INCLUDE = `pkg-config opencv --cflags`
OPENCV         =  $(OPENCV_INCLUDE)  $(OPENCV_LIBS)

##################
#      BOOST     #
##################
BOOST_LIBS    = -L/usr/lib -lboost_system -lboost_filesystem
BOOST_INCLUDE = -I/usr/include 
BOOST         = $(BOOST_INCLUDE) $(BOOST_LIBS)

####################
#       GDAL       #
####################
GDAL_LIBS       = -lgdal
GDAL_INCLUDE    = -I/usr/include/gdal
GDAL            = $(GDAL_INCLUDE) $(GDAL_LIBS)

#####################
#      GEOIMAGE     #
#####################
GEOIMAGE_LIBS    = -L/opt/local/lib -lgeoimage
GEOIMAGE_INCLUDE = -I/opt/local/include
GEOIMAGE         = $(GEOIMAGE_LIBS) $(GEOIMAGE_INCLUDE)



LIBS = $(OPENCV) $(BOOST) $(GEOIMAGE) $(GDAL)
INCL = $(OPENCV_INCLUDE) $(BOOST_INCLUDE) $(GEOIMAGE_INCLUDE) $(GDAL_INCLUDE)

EXEC = $(BIN)/orthoproject

CPP=g++
FLAGS = -Wall -g

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
		 $(BUILD)/dem.o \
		 $(BUILD)/ProgressBar.o \
		 $(BUILD)/Logger.o \
		 $(BUILD)/OrthoExceptions.o

HEADERS= $(CORE)/Configuration.hpp \
		 $(CORE)/Parser.hpp \
		 $(ORTHO)/Orthorectify.hpp \
		 $(ORTHO)/Perspective.hpp \
		 $(MATH)/Quaternion.hpp \
		 $(MATH)/Vector.hpp \
		 $(MATH)/Geometry.hpp \
		 $(CORE)/Utilities.hpp \
		 $(CORE)/dem.hpp \
		 $(CORE)/ProgressBar.hpp \
		 $(CORE)/Logger.hpp \
		 $(CORE)/OrthoExceptions.hpp


# Create required directory structure
orthoproject: build bin $(EXEC)

build:
	echo "Constructing build directory"
	mkdir -p build

bin: 
	echo "Constructing bin directory"
	mkdir -p bin

###############################
#          Executable         #
###############################
$(EXEC): $(OBJECTS) $(HEADERS)
	$(CPP) $(OBJECTS) -o $@ $(LIBS) $(FLAGS)


###############################
#          Main Driver        #
###############################
$(BUILD)/orthoproject.o: $(SRC)/orthoproject.cpp  $(HEADERS)
	$(CPP) $< -c -o $@ $(INCL) $(FLAGS)


# Configuration
$(BUILD)/Configuration.o: $(CORE)/Configuration.cpp $(HEADERS)
	$(CPP) $< -c -o $@ $(INCL) $(FLAGS)

# Config Parser
$(BUILD)/Parser.o: $(CORE)/Parser.cpp $(HEADERS)
	$(CPP) $< -c -o $@ $(INCL) $(FLAGS)

# Orthorectification
$(BUILD)/Orthorectify.o: $(ORTHO)/Orthorectify.cpp $(HEADRES)
	$(CPP) $< -c -o $@ $(INCL) $(FLAGS)

# Perspective Code
$(BUILD)/Perspective.o: $(ORTHO)/Perspective.cpp $(HEADERS)
	$(CPP) $< -c -o $@ $(INCL) $(FLAGS)

# Quaternion Code
$(BUILD)/Quaternion.o: $(MATH)/Quaternion.cpp $(HEADERS)
	$(CPP) $< -c -o $@ $(INCL) $(FLAGS)

#Vector Code
$(BUILD)/Vector.o: $(MATH)/Vector.cpp $(HEADERS)
	$(CPP) $< -c -o $@ $(INCL) $(FLAGS)

# Geometry code
$(BUILD)/Geometry.o: $(MATH)/Geometry.cpp $(HEADERS)
	$(CPP) $< -c -o $@ $(INCL) $(FLAGS)

# Utility Functions
$(BUILD)/Utilities.o: $(CORE)/Utilities.cpp $(HEADERS)
	$(CPP) $< -c -o $@ $(INCL) $(FLAGS)

# DEM Functions
$(BUILD)/dem.o: $(CORE)/dem.cpp $(HEADERS)
	$(CPP) $< -c -o $@ $(INCL) $(FLAGS)

# Progress Bar
$(BUILD)/ProgressBar.o: $(CORE)/ProgressBar.cpp $(HEADERS)
	$(CPP) $< -c -o $@ $(INCL) $(FLAGS)

# Logger
$(BUILD)/Logger.o: $(CORE)/Logger.cpp $(HEADERS)
	$(CPP) $< -c -o $@ $(INCL) $(FLAGS)

# OrthoExceptions
$(BUILD)/OrthoExceptions.o: $(CORE)/OrthoExceptions.cpp $(HEADERS)
	$(CPP) $< -c -o $@ $(INCL) $(FLAGS)


####################################
#       UNIT TEST FRAMEWORK        #
####################################
check:
	make -f $(SRC)/Makefile check

####################################
#          DOCUMENTATION           #
####################################
docs:
	doxygen Doxyfile

####################################
#              CLEANUP             #
####################################
clean: 
	rm -r $(EXEC) $(BUILD)/*


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
		 $(BUILD)/Geometry.o

HEADERS= $(CORE)/Configuration.hpp \
		 $(CORE)/Parser.hpp \
		 $(ORTHO)/Orthorectify.hpp \
		 $(ORTHO)/Perspective.hpp \
		 $(MATH)/Quaternion.hpp \
		 $(MATH)/Vector.hpp \
		 $(MATH)/Geometry.hpp

###############################
#          Executable         #
###############################
$(EXEC): $(OBJECTS) $(HEADERS)
	g++ $(OBJECTS) -o $@ $(LIBS)


###############################
#          Main Driver        #
###############################
$(BUILD)/orthoproject.o: orthoproject.cpp  $(HEADERS)
	g++ $< -c -o $@ $(INCL)


# Configuration
$(BUILD)/Configuration.o: $(CORE)/Configuration.cpp $(HEADERS)
	g++ $< -c -o $@ $(INCL)

# Config Parser
$(BUILD)/Parser.o: $(CORE)/Parser.cpp $(HEADERS)
	g++ $< -c -o $@ $(INCL)

# Orthorectification
$(BUILD)/Orthorectify.o: $(ORTHO)/Orthorectify.cpp $(HEADRES)
	g++ $< -c -o $@ $(INCL)

# Perspective Code
$(BUILD)/Perspective.o: $(ORTHO)/Perspective.cpp $(HEADERS)
	g++ $< -c -o $@ $(INCL)

# Quaternion Code
$(BUILD)/Quaternion.o: $(MATH)/Quaternion.cpp $(HEADERS)
	g++ $< -c -o $@ $(INCL)

#Vector Code
$(BUILD)/Vector.o: $(MATH)/Vector.cpp $(HEADERS)
	g++ $< -c -o $@ $(INCL)

# Geometry code
$(BUILD)/Geometry.o: $(MATH)/Geometry.cpp $(HEADERS)
	g++ $< -c -o $@ $(INCL)


clean: 
	rm -r $(EXEC) $(BUILD)/*




# Directory Labels, use this to avoid renaming every item in the makefile
BIN=bin
BLD=build
SRC=src
DIRS=$(BIN) $(BLD) $(SRC)

# name of executable
EXEC=$(BIN)/frame_analyzer

# name of config file
CFG=frame_analyzer.cfg

#-----------------------------------------------#
#-               Main Requirements             -#
#-----------------------------------------------#
# List of objects to build
OBJECTS=$(BLD)/ProgressBar.o    $(BLD)/FilePtr.o    $(BLD)/Core.o    $(BLD)/Context.o   $(BLD)/TACID.o   $(BLD)/Metrics.o    $(BLD)/frame_analyzer.o $(BLD)/Parser.o $(BLD)/Options.o $(BLD)/Camera.o $(BLD)/IO.o 
HEADERS=$(SRC)/ProgressBar.hpp  $(SRC)/FilePtr.hpp  $(SRC)/Core.hpp  $(SRC)/Context.hpp $(SRC)/TACID.hpp $(SRC)/Metrics.hpp  $(SRC)/Camera.hpp $(SRC)/Options.hpp $(SRC)/Parser.hpp  $(SRC)/IO.hpp

# Desired C++ Compiler
CPP=g++

# Desired compilation flags
FLAGS= -Wall -O3

#-----------------------------------------#
#-              Dependencies             -#
#-----------------------------------------#
#BOOST_DIR=/usr/local
BOOST_DIR=/usr
BOOST_INCL=-I$(BOOST_DIR)/include


ifeq ($(OS),RHEL)
	BOOST_LIBS=$(BOOST_DIR)/lib64/libboost_system.a $(BOOST_DIR)/lib64/libboost_filesystem.a
else
	BOOST_LIBS=$(BOOST_DIR)/lib/libboost_system.a $(BOOST_DIR)/lib/libboost_filesystem.a
endif

INCL= $(BOOST_INCL)
LIBS= $(BOOST_LIBS)

#-----------------------------------------#
#-             Main Project              -#
#-----------------------------------------#
$(EXEC): $(DIRS) $(OBJECTS) $(HEADERS)
	$(CPP) $(OBJECTS) -o $@ $(LIBS) $(FLAGS)


#-----------------------------------------#
#-             Build Objects             -#     
#-----------------------------------------#
# frame analyzer
$(BLD)/frame_analyzer.o: $(SRC)/frame_analyzer.cpp $(HEADERS)
	$(CPP) $< -c -o $@  $(INCL)  $(FLAGS)


# Parser
$(BLD)/Parser.o: $(SRC)/Parser.cpp $(HEADERS)
	$(CPP) $< -c -o $@  $(INCL)  $(FLAGS)

# Options
$(BLD)/Options.o: $(SRC)/Options.cpp $(HEADERS)
	$(CPP) $< -c -o $@  $(INCL)  $(FLAGS)

# Camera
$(BLD)/Camera.o: $(SRC)/Camera.cpp $(HEADERS)
	$(CPP) $< -c -o $@  $(INCL)  $(FLAGS)

# IO
$(BLD)/IO.o: $(SRC)/IO.cpp $(HEADERS)
	$(CPP) $< -c -o $@  $(INCL)  $(FLAGS)

# TACID
$(BLD)/TACID.o: $(SRC)/TACID.cpp $(HEADERS)
	$(CPP) $< -c -o $@  $(INCL)  $(FLAGS)

# Context
$(BLD)/Context.o: $(SRC)/Context.cpp $(HEADERS)
	$(CPP) $< -c -o $@  $(INCL)  $(FLAGS)

# Core
$(BLD)/Core.o: $(SRC)/Core.cpp $(HEADERS)
	$(CPP) $< -c -o $@  $(INCL)  $(FLAGS)

# Metrics
$(BLD)/Metrics.o: $(SRC)/Metrics.cpp $(HEADERS)
	$(CPP) $< -c -o $@  $(INCL)  $(FLAGS)

# FilePtr
$(BLD)/FilePtr.o: $(SRC)/FilePtr.cpp $(HEADERS)
	$(CPP) $< -c -o $@  $(INCL)  $(FLAGS)

# ProgressBar
$(BLD)/ProgressBar.o: $(SRC)/ProgressBar.cpp $(HEADERS)
	$(CPP) $< -c -o $@  $(INCL)  $(FLAGS)

#-----------------------------------------#
#-          Required Directories         -#
#-----------------------------------------#
$(BIN):
	mkdir -p $(BIN)

$(BLD):
	mkdir -p $(BLD)

$(SRC):
	mkdir -p $(SRC)


#-----------------------------------------#
#-          Clean Up Everything          -#
#-----------------------------------------#
clean:
	rm -r $(EXEC)  $(BLD)/*

#-----------------------------------------#
#-         Install to a machinie          #
#-----------------------------------------#
install:
	mkdir -p frame_analyzer
	cp $(EXEC) frame_analyzer
	cp $(CFG)  frame_analyzer
	scp -r frame_analyzer root@gsam-server:	

help:
	echo "make OS=<RHEL>, otherwise leave OS blank"


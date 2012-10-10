


# Directory Labels, use this to avoid renaming every item in the makefile
BIN=bin
BLD=build
SRC=src
DIRS=$(BIN) $(BLD) $(SRC)

# name of executable
EXEC=$(BIN)/frame_analyzer


#-----------------------------------------------#
#-               Main Requirements             -#
#-----------------------------------------------#
# List of objects to build
OBJECTS=$(BLD)/frame_analyzer.o $(BLD)/Parser.o $(BLD)/Options.o $(BLD)/Camera.o $(BLD)/IO.o
HEADERS=$(SRC)/TACID.hpp $(SRC)/Camera.hpp $(SRC)/Options.hpp $(SRC)/Parser.hpp  $(SRC)/IO.hpp

# Desired C++ Compiler
CPP=g++

# Desired compilation flags
FLAGS= -Wall -O3

#-----------------------------------------#
#-              Dependencies             -#
#-----------------------------------------#
BOOST_INCL=-I/usr/include
BOOST_LIBS=/usr/lib64/libboost_system.a /usr/lib64/libboost_filesystem.a


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
	rm -r $(BIN)/* $(BLD)/*



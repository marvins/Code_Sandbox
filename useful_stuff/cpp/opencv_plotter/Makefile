
OPENCV_INCL=`pkg-config opencv --cflags`
OPENCV_LIBS=`pkg-config opencv --libs`
OPENCV=$(OPENCV_INCL) $(OPENCV_LIBS)

BOOST_INCL=-I/opt/local/include


SRC=OpenCVPlotTools
BLD=build
BIN=bin

INCL=$(OPENCV_INCL) $(BOOST_INCL) -I.
LIBS=$(OPENCV_LIBS) $(BOOST_LIBS)

CPP=g++

FLAGS=-g -Wall

PLOTLIB=$(BLD)/OpenCVPlotTools.a

DIRS=$(BIN) $(BLD)

OBJECTS=$(BLD)/Figure.o    $(BLD)/PlotData.o    $(BLD)/PlotMath.o    $(BLD)/PlotUtils.o
HEADERS=$(SRC)/Figure.hpp  $(SRC)/PlotData.hpp  $(SRC)/PlotMath.hpp  $(SRC)/PlotUtils.hpp


all: $(PLOTLIB)
	cp $(SRC)/OpenCVPlotTools.hpp $(BLD)/OpenCVPlotTools.hpp
	
	mkdir -p $(BLD)/OpenCVPlotTools
	cp $(SRC)/*.hpp $(BLD)/OpenCVPlotTools

#############################################
#            Demo Executable                #
#############################################
$(BIN)/demo: $(DIRS) demo.cpp $(HEADERS)
	$(CPP) demo.cpp -o $@ $(PLOTLIB) $(FLAGS) -Ibuild `pkg-config opencv --cflags --libs`

########################################
#           Static Library             #
########################################
$(PLOTLIB): $(DIRS) $(HEADERS) $(OBJECTS)
	ar -cvq $@ $(OBJECTS)

# directories
bin: 
	mkdir -p bin
build:
	mkdir -p build


# Figure Class
$(BLD)/Figure.o: $(SRC)/Figure.cpp $(HEADERS)
	$(CPP) $< -c -o $@  $(FLAGS) $(INCL)

# PlotData Class
$(BLD)/PlotData.o: $(SRC)/PlotData.cpp $(HEADERS)
	$(CPP) $< -c -o $@  $(FLAGS) $(INCL)

#Plot_math
$(BLD)/PlotMath.o: $(SRC)/PlotMath.cpp $(HEADERS)
	$(CPP) $< -c -o $@  $(FLAGS) $(INCL)

#Plot utils
$(BLD)/PlotUtils.o: $(SRC)/PlotUtils.cpp $(HEADERS)
	$(CPP) $< -c -o $@  $(FLAGS) $(INCL)

########################################
#                Clean                 #
########################################
clean:
	rm -r $(BIN)/* $(BLD)/*


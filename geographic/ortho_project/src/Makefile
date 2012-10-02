BLD=build
SRC=src
UBLD=$(BLD)/utest
UTST=$(SRC)/unit_testing

FLAGS= -Wall

OBJECTS= $(UBLD)/unit_test.o $(UBLD)/Logger.o  $(UBLD)/Color.o
HEADERS= $(UTST)/Logger.hpp $(UTST)/Color.hpp


check: $(BLD) $(UBLD) $(UBLD)/unit_test $(OBJECTS)
	./$(UBLD)/unit_test
	
$(BLD):
	mkdir -p $(BLD)

$(UBLD):
	mkdir -p $(UBLD)

$(UBLD)/unit_test: $(OBJECTS) $(HEADERS)
	g++ $(OBJECTS) -o $@ $(FLAGS)

$(UBLD)/unit_test.o: $(SRC)/unit_test.cpp
	g++ $< -c -o $@ $(FLAGS)

$(UBLD)/Logger.o: $(UTST)/Logger.cpp $(HEADERS)
	g++ $< -c -o $@ $(FLAGS)

$(UBLD)/Color.o: $(UTST)/Color.cpp $(HEADERS)
	g++ $< -c -o $@ $(FLAGS)


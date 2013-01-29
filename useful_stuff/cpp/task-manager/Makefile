
HEADERS=src/*.hpp  src/core/*.hpp  src/ui/*.hpp
SOURCES=src/*.cpp  src/core/*.cpp  src/ui/*.cpp

LIBS=-L/opt/local/lib -lboost_system -lboost_filesystem -lncurses

bin/task-manager: $(SOURCES) $(HEADERS)
	g++ $(SOURCES) -o $@ $(LIBS)

clean:
	rm bin/task-manager

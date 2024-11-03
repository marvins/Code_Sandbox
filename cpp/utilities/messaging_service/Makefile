
HEADERS=BlockingQueue.hpp \
        MessageService.hpp \
		Utilities.hpp

SOURCES=MessageService.cpp

OBJECTS=MessageService.o

message_service_demo: message_service_demo.cpp $(OBJECTS)
	g++ $< -o $@ -std=c++11 $(OBJECTS)

MessageService.o: MessageService.cpp $(HEADERS)
	g++ -o $@ -c $(SOURCES) -std=c++11

clean:
	rm *.o message_service_demo

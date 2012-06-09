all: hello_geoworld dted_example

hello_geoworld: hello_geoworld.cpp
	g++ hello_geoworld.cpp -o hello_geoworld `pkg-config opencv --cflags --libs` -I/opt/local/include -L/opt/local/lib -lgeoimage_DEBUG -g

dted_example:   dted_example.cpp
	g++ dted_example.cpp -o dted_example `pkg-config opencv --cflags --libs` -I/opt/local/include -L/opt/local/lib -lgeoimage_DEBUG -g

	
clean:
	rm -rf hello_geoworld dted_example *.dSYM

all: hello_geoworld dted_example simple_writer

GDAL = -I/usr/include/gdal -lgdal

hello_geoworld: hello_geoworld.cpp
	g++ hello_geoworld.cpp -o hello_geoworld `pkg-config opencv --cflags --libs` $(GDAL) -I/opt/local/include -lgeoimage 

dted_example:   dted_example.cpp
	g++ dted_example.cpp   -o dted_example   `pkg-config opencv --cflags --libs` $(GDAL) -I/opt/local/include -lgeoimage

simple_writer:  simple_writer.cpp
	g++ simple_writer.cpp -o simple_writer  `pkg-config opencv --cflags --libs` $(GDAL) -I/opt/local/include -lgeoimage
	
clean:
	rm -rf hello_geoworld dted_example *.dSYM

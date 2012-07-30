all: hello_geoworld dted_example simple_writer

GDAL = -I/usr/include/gdal -lgdal
GEOIMAGE = -I/opt/local/include -L/opt/local/lib -lgeoimage
OPENCV   = `pkg-config opencv --cflags --libs`

hello_geoworld: hello_geoworld.cpp
	g++ hello_geoworld.cpp -o hello_geoworld $(OPENCV) $(GDAL) $(GEOIMAGE)

dted_example:   dted_example.cpp
	g++ dted_example.cpp   -o dted_example   $(OPENCV) $(GDAL) $(GEOIMAGE)

simple_writer:  simple_writer.cpp
	g++ simple_writer.cpp -o simple_writer   $(OPENCV) $(GDAL) $(GEOIMAGE)
	
clean:
	rm -rf hello_geoworld dted_example simple_writer *.dSYM

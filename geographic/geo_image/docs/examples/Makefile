all: GS2_Header_Demo hello_geoworld dted_example simple_writer advanced_geoimage coordinate_conversion

BOOST    = -lboost_system -lboost_filesystem
GDAL = -I/usr/include/gdal -lgdal
GEOIMAGE = -I/opt/local/include -L/opt/local/lib -lgeoimage
OPENCV   = `pkg-config opencv --cflags --libs`

hello_geoworld: hello_geoworld.cpp
	g++ hello_geoworld.cpp -o hello_geoworld $(OPENCV) $(GDAL) $(GEOIMAGE)

dted_example:   dted_example.cpp
	g++ dted_example.cpp   -o dted_example   $(OPENCV) $(GDAL) $(GEOIMAGE)

simple_writer:  simple_writer.cpp
	g++ simple_writer.cpp -o simple_writer   $(OPENCV) $(GDAL) $(GEOIMAGE)

advanced_geoimage: advanced_geoimage.cpp
	g++ advanced_geoimage.cpp -o advanced_geoimage $(BOOST) $(OPENCV) $(GDAL) $(GEOIMAGE)

coordinate_conversion: coordinate_conversion.cpp
	g++ coordinate_conversion.cpp -o coordinate_conversion $(GDAL) $(GEOIMAGE)

GS2_Header_Demo: GS2_Header_Demo.cpp
	g++ GS2_Header_Demo.cpp -o GS2_Header_Demo $(OPENCV) $(GDAL) $(GEOIMAGE)

clean:
	rm -rf hello_geoworld dted_example simple_writer advanced_geoimage coordinate_conversion *.dSYM

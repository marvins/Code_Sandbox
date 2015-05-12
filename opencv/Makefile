
LIBS=-L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -lopencv_stitching

INCL=-I/usr/local/include

stitcher: stitcher.cpp
	g++ $< -o $@ $(LIBS) $(INCL)


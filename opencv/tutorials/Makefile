OPENCV_LIBS = -L/opt/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc
OPENCV = -I/opt/local/include $(OPENCV_LIBS)

SOURCES = Frequency_Transforms.cpp

HEADERS = Frequency_Transforms.h

FLAGS = -Wall -Wextra

EXECS =  Gaussian_Blur Median_Blur Block_Smoothing\
			Pyramid_Sample Image_IO Video_Play\
			Sobel_Detection Scharr_Detection\
			FFT_Process Unsharp_Mask High_Boost Flood_Fill\
			Connected_Components FFT_gaussian Harris_Corner\
			Corner_Harris_OpenCV

all: $(EXECS) 

Frequency_Transforms.o: Frequency_Transforms.h Frequency_Transforms.cpp
	g++ Frequency_Transforms.cpp -c Frequency_Transforms.o $(FLAGS) $(OPENCV_LIBS)


# Create Gaussian Blur
Gaussian_Blur: Gaussian_Blur.cpp
	g++ Gaussian_Blur.cpp -o Gaussian_Blur $(OPENCV) $(FLAGS)

# Create Median Blur
Median_Blur: Median_Blur.cpp
	g++ Median_Blur.cpp -o Median_Blur $(OPENCV) $(FLAGS)

# Create Block_Smoothing
Block_Smoothing: Block_Smoothing.cpp
	g++ Block_Smoothing.cpp -o Block_Smoothing $(OPENCV) $(FLAGS)
			
Pyramid_Sample: Pyramid_Sample.cpp
	g++ Pyramid_Sample.cpp -o Pyramid_Sample $(OPENCV) $(FLAGS)

Image_IO: Image_IO.cpp
	g++ Image_IO.cpp -o Image_IO $(OPENCV) $(FLAGS)

Video_Play: Video_Play.cpp
	g++ Video_Play.cpp -o Video_Play $(FLAGS) $(OPENCV)

Sobel_Detection: Sobel_Detection.cpp
	g++ Sobel_Detection.cpp -o Sobel_Detection $(FLAGS) $(OPENCV)

Scharr_Detection: Scharr_Detection.cpp
	g++ Scharr_Detection.cpp -o Scharr_Detection $(FLAGS) $(OPENCV)

Fourier_Transform: Fourier_Transform.cpp
	g++ Fourier_Transform.cpp -o Fourier_Transform $(FLAGS) $(OPENCV)

Unsharp_Mask: Unsharp_Mask.cpp
	g++ Unsharp_Mask.cpp -o Unsharp_Mask $(FLAGS) $(OPENCV)

High_Boost: High_Boost.cpp
	g++ High_Boost.cpp -o High_Boost 	$(FLAGS) $(OPENCV)

FFT_Process: FFT_Process.cpp $(SOURCES) $(HEADERS)
	g++ FFT_Process.cpp -o FFT_Process 	$(FLAGS) $(OPENCV) Frequency_Transforms.cpp

Flood_Fill: Flood_Fill.cpp
	g++ Flood_Fill.cpp -o Flood_Fill $(FLAGS) $(OPENCV)

Connected_Components: Connected_Components.cpp
	g++ Connected_Components.cpp -o Connected_Components $(FLAGS) $(OPENCV) 

FFT_gaussian: FFT_gaussian.cpp $(SOURCES) $(HEADERS)
	g++ FFT_gaussian.cpp -o FFT_gaussian $(FLAGS) $(OPENCV) Frequency_Transforms.cpp

Harris_Corner: Harris_Corner.cpp 
	g++ Harris_Corner.cpp -o Harris_Corner $(FLAGS) $(OPENCV)

Corner_Harris_OpenCV: Corner_Harris_OpenCV.cpp
	g++ Corner_Harris_OpenCV.cpp -o Corner_Harris_OpenCV $(OPENCV) $(FLAGS)

ImageStitching: ImageStitching.cpp $(SOURCES) $(HEADERS)
	g++ ImageStitching.cpp -o ImageStitching $(FLAGS) $(OPENCV) lmfit/lmmin.c

clean: 
	rm $(EXECS) *.o

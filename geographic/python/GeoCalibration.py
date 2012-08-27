#! /usr/bin/env python
import cv, cv2, geoimage, numpy as np, sys


gimage = None


def main():
	
	global gimage
	
	# Create the GeoImage Object
	gimage = geoimage.GeoImage( sys.argv[1] )
	
	# Pull the OpenCV Image and convert to uint8
	image = np.dot(gimage.image, 1.0/(2**8)).astype(np.uint8)

	# Show the image
	cv.NamedWindow( 'output' )
	cv2.imshow( 'output', image )
	cv.WaitKey(0)

	# Write to file
	cv2.imwrite( 'output.jpg', image )


if __name__ == '__main__':

	main()


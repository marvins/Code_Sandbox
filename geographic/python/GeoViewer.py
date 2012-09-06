#! /usr/bin/env python
import cv, cv2, geoimage, numpy as np, sys


def main():
	
	global gimage
	
	# Create the GeoImage Object
	gimage = geoimage.GeoImage( sys.argv[1] ).image
	
	# Pull the OpenCV Image and convert to uint8
	image = np.dot(gimage, 1.0/(2**8)).astype(np.uint8)
	

	# Create a transformation


	# Write to file
	cv2.imwrite( sys.argv[2], image )


if __name__ == '__main__':

	main()


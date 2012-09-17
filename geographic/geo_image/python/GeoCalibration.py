#! /usr/bin/env python
import cv, cv2, geoimage, numpy as np, sys

gimage = None
op = None

class Options:
	
	Po      = None
	cols    = None
	rows    = None
	pix2img = np.eye(3,3)

	origin_lat = 38.846442
	origin_lon = -104.778871
	origin_alt = 6506        # Meters


def main():
	
	global gimage
	
	# Create the GeoImage Object
	image = cv2.imread('input.jpg') #geoimage.GeoImage( sys.argv[1] )
	
	# Pull the OpenCV Image and convert to uint8
	#image = np.dot(gimage, 1.0/(2**8)).astype(np.uint8)
	

	# Create a transformation
	global op
	op = Options()

 	# Grab the width and height of the image
	op.cols = float(len(image[0]))
	op.rows = float(len(image))

	# Create the origin point
	op.Po = np.mat( [ [ op.cols/ 2 ],  
	                  [ op.rows/ 2 ], 
					  [      1            ] ])
	
	# Create the transformation from pixel plane to camera plane
	op.pix2img *= np.mat( [ [ 1/op.cols,       0    ,      0    ],
	                      [       0    ,   1/op.rows,      0    ],
						  [       0    ,       0    ,      1    ] ] ) 
	op.pix2img *= np.mat( [ [     1    ,       0    , -op.Po[0] ],
			              [       0    ,       1    , -op.Po[1] ],
						  [       0    ,       0    ,      0    ]]);
	
	
	# We need to rotate the image


	# Write to file
	cv2.imwrite( 'output.jpg', image )


if __name__ == '__main__':

	main()


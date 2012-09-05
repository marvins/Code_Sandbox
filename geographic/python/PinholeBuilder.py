#! /usr/bin/env python
import xml.etree.ElementTree as xml, math, cv, cv2, geoimage, numpy as np, sys

rows = 1000
cols = 1000
	
class Options:

	config_filename = ''
	focal_length = 1
	
	cam_alt        = 1800
	
	rotation_angle = 0
	rotation_axis  = [1, 0, 0]
	scale          = [1, 1, 1]
	translation    = [0, 0, 0]

	image_filename = 'test'


	def __str__(self):
		strout  = 'Configuration Information\n'
		strout += '\n'
		strout += 'Rotation Angle: ' + str(self.rotation_angle) + '\n'
		strout += 'Rotation Axis : ' + str(self.rotation_axis[0]) + ', ' + str(self.rotation_axis[1]) + ', ' + str(self.rotation_axis[2]) + '\n'
		strout += 'Scale         : ' + str(self.scale[0])         + ', ' + str(self.scale[1] )        + ', ' + str(self.scale[2]) + '\n'
		strout += 'Translation   : ' + str(self.translation[0])   + ', ' + str(self.translation[1])   + ', ' + str(self.translation[2]) + '\n'
		return strout
	
	def write_configuration(self):
		
		# Open Configuration File
		FILE = open(image_filename + '_config.xml', 'w')
	
		# Write the First Tag
		FILE.write('<PinholeBuilder>')

		# Close the document
		FILE.write('</PinholeBuilder>')
		FILE.close()


def Img2Cam( point, size, Po ):
	
	# Create the transformation from pixel plane to camera plane
	scale = np.mat( [ [ 1.0/size[0],           0 ,         0 , -Po[0] ],
	                [            0 ,  1.0/size[1],         0 , -Po[1] ],
	                [            0 ,           0 ,  1/size[2],  Po[2] ],
					[            0 ,           0 ,         0 ,     1  ]]) 
	return scale * point


class Quaternion:

	r = 1
	i = np.zeros((3,1), float)

	def __init__( self, angle, axis ):
		
		# Make sure we have a unit axis
		uaxis = axis / np.linalg.norm(axis)
		
		# convert angle to radians
		theta = angle * math.pi / 180.0

		# Create quaternion
		self.r = math.cos(theta/2.0)
		self.i = np.array([
			math.sin(theta/2.0)*uaxis[0],
			math.sin(theta/2.0)*uaxis[1],
			math.sin(theta/2.0)*uaxis[2]])	
	
	def __str__(self):
		return str(self.r) + ', ' + str(self.i)
	
	def mag2(self):
		return ((self.r*self.r) + (np.dot(self.i,self.i)))

	def mag( self ):
		return math.sqrt(self.mag2())

	def x(self):
		return self.i[0]

	def y(self):
		return self.i[1]

	def z(self):
		return self.i[2]

	def get_rotation_matrix(self):
		
		Matrix = np.mat([[ 1 - 2*self.y()*self.y() - 2*self.z()*self.z(), 2*self.x()*self.y() + self.r*self.z()         , 2*self.x()*self.z() - 2*self.r*self.y()                      , 0],
		                 [ 2*self.x()*self.y() - 2*self.r*self.z()      , 1 - 2*self.x()*self.x() - 2*self.z()*self.z() , 2*self.y()*self.z() + 2*self.y()*self.z() + 2*self.r*self.x(), 0],
						 [ 2*self.x()*self.z() - 2*self.r*self.y()      , 2*self.y()*self.z() - 2*self.r*self.x()       , 1 - 2*self.x()*self.x() - 2*self.y()*self.y()                , 0],
						 [                                             0,                                              0,                                                             0, 1]])
		return Matrix





#######################################################
#                     Main Driver                     #
#######################################################
def main():
	"""
	Main Driver for the Pinhole Camera Model Test Bench. 
	"""

	# Create the config options
	options = Options( )
	
	# Print the configuration for good measure
	print options

	# Build translation matrix
	translation = np.mat([ [1, 0, 0, options.translation[0]], 
	                       [0, 1, 0, options.translation[1]], 
						   [0, 0, 1, options.translation[2]],
						   [0, 0, 0,                     1 ]])	


	##################################################
	#             Create Test Ortho Image            #
	##################################################
	print 'Creating test image'
	
	pre_warp = None
	
	# Create an empty OpenCV Image
	pre_warp = np.ones(  (rows, cols, 1), np.uint8)
	pre_warp *= 255
		
	pre_dem  = np.zeros( (rows, cols, 1), np.uint8)
		
	# Fill the image with empty values
	for x in xrange( 0, cols):
		for y in xrange( 0, rows):
	
			# Set the positional color
			pre_warp[y][x] = min(20 + 255*(x+y)/float(cols+rows), 255)
	
			if ( x > 280 and x < 320 ) or ( x > 680 and x < 720 ):
				pre_warp[y][x] = 0

			if  ( x > 490 and x < 510 ) and (
				( y > 100 and y < 200  ) or
				( y > 300 and y < 400  ) or
				( y > 500 and y < 600  ) or
				( y > 700 and y < 800  ) or
				( y > 900 and y < 1000 )):
				pre_warp[y][x] = 0
				
			# Draw An Occlusion
			if ( x > 860 and x < 960 ) and ( y > 600 and y < 690 ):
				pre_warp[y][x] = 250
				pre_dem[y][x]  = 250

			# Draw an object to be occluded
			if ( x > 870 and x < 950 ) and ( y > 520 and y < 580 ):
				pre_warp[y][x] = 0
	

	# Create the OpenCV Windows
	cv.NamedWindow('Test Orthographic Image')
	cv.NamedWindow('Test Orthographic DEM')
	cv2.imshow('Test Orthographic Image', pre_warp)
	cv2.imshow('Test Orthographic DEM'  , pre_dem)
	cv.WaitKey(0)
	cv2.destroyWindow('Test Orthographic Image')
	cv2.destroyWindow('Test Orthographic DEM')
	cv2.imwrite( options.image_filename + '_prewarp.jpg', pre_warp)
	cv2.imwrite( options.image_filename + '_dem.jpg',     pre_dem)

	##################################################
	#             Create Perspective Image           #
	##################################################
	print 'converting to perspective camera'
	# Now lets specify some camera parameters
	
	# This is the vector which defines the direction of the camera
	Vf = np.mat([[0], [0], [1], [0]])
	
	# Lets adjust the direction of the camera 
	rot_axis = Quaternion( options.rotation_angle, options.rotation_axis).get_rotation_matrix()
	Vf = rot_axis * Vf


	# This is the focal length
	f = options.focal_length
	
	# Height of the camera above the datum
	H = options.cam_alt

	# This is the origin of the camera
	# - For this camera system, this is the focal point or the
	#   point of convergence. 
	Po = np.mat([ [0], [0], [0], [1]])  # This should end up as lon, lat, elevation


	# We want to apply the camera translation here to the origin of the camera
	Po = translation * Po
	

	# This is the point of intersection between the camera vector and the image plane. 
	# -  This is also known as the principle point or "center"
	Pp = Po + (f*Vf)


	# - This is the principle point given in pixels
	Ip = np.mat([ [(cols/2.0)/cols], [(rows/2.0)/rows], [f], [1]])
	
	Ph = np.mat([ [cols/2.0], [rows/2.0], [0], [1]])
	# This is the nadir point
	# - This represents the intersection of the earth plane and the camera view vector
	Pn = Po + (H*Vf)

	# Create OpenCV Image
	perspective = np.zeros( (rows, cols, 1), np.uint8)




	# Convert Orthographic Image into Perspective Image
	cnt = 0
	N = Vf
	P1 = Po
	P3 = np.mat([ [1], [1], [H], [1]])
	PT = np.dot( N.T, P3 - P1)[0,0]
	for x in xrange(0, cols):
		for y in xrange(0, rows):
			
			# Convert Output Position to Camera Coordinates
			P_cam = Img2Cam( np.mat([[x],[y],[f], [1]]),
			                    (cols, rows, H),
					               Ip)
			
			#print x, y
			#print 'Pcam: ', P_cam.T

			# Compute Intersection of ray with earth plane
			u = PT/(np.dot( N.T, (P_cam - P1)))
			Pnt = P1 + u[0,0]*(P_cam-P1) + Ph
			
			#print 'Pnt: ', Pnt.T
			#raw_input('pause')
			
			# Compute the image value of P on the Ortho Image
			if Pnt[0,0] >= 0 and Pnt[1,0] >= 0 and Pnt[0,0] < cols and Pnt[1,0] < rows:
				perspective[y][x] = pre_warp[int(Pnt[1,0])][int(Pnt[0,0])] 
			else:
				perspective[y][x] = 0
			
			if cnt%10000 == 0:
				sys.stdout.write('x')
				sys.stdout.flush()

			cnt += 1


	print ''

	cv.NamedWindow('Conversion to Perspective')
	cv2.imshow('Conversion to Perspective', perspective)
	cv.WaitKey(0)
	cv2.imwrite( options.image_filename + '_image.jpg', perspective)

	


if __name__ == '__main__':
	main()



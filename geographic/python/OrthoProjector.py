#! /usr/bin/env python
import sys, math, xml.etree.ElementTree as xml, cv, cv2, numpy as np

image = None
rows = 0
cols = 0

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
		
		Matrix = np.mat([[ 1 - 2*self.y()*self.y() - 2*self.z()*self.z(), 2*self.x()*self.y() + 2*self.r*self.z()       , 2*self.x()*self.z() - 2*self.r*self.y()       , 0],
		                 [ 2*self.x()*self.y() - 2*self.r*self.z()      , 1 - 2*self.x()*self.x() - 2*self.z()*self.z() , 2*self.y()*self.z() + 2*self.r*self.x()       , 0],
						 [ 2*self.x()*self.z() + 2*self.r*self.y()      , 2*self.y()*self.z() - 2*self.r*self.x()       , 1 - 2*self.x()*self.x() - 2*self.y()*self.y() , 0],
						 [                                             0,                                              0,                                             0 , 1]])
		return Matrix



class Options:
	
	image_filename  = ''
	config_filename = ''
	
	image = None
	cols  = 0
	rows  = 0
	
	f = 1
	Vf_in  = np.mat([[0], [0], [-1], [0]])
	Vf_out = np.mat([[0], [0], [-1], [0]])

	img2world = np.eye(4,4)

	Po = np.mat([[0],[0],[0],[1]])

	rotation_angle = 0
	rotation_axis  = [0, 1, 0]
	rotation       = None

	def __init__(self, config_filename ):
		
		# Set the config filename
		self.config_filename = config_filename
		
		# Open the config file
		root = xml.parse(self.config_filename)
		
		# Load the image name
		if root.find('image').attrib.get('name'):
			self.image_filename = root.find('image').attrib.get('name')

		# Load the Focal Length
		if root.find('camera/focal_length').attrib.get('value'):
			self.f = float(root.find('camera/focal_length').attrib.get('value'))

		# Load the image
		global image
		global rows
		global cols
		image = cv2.imread( self.image_filename, 0)
		cols = len(image[0])
		rows = len(image)
	

		# Define the cam2world variable
		self.img2world = np.mat([ [ 1.0/cols,       0 ,     0 ,   -0.5  ],
		                          [       0 , 1.0/rows,     0 ,   -0.5  ],
								  [       0 ,       0 ,     1 ,     0   ],
								  [       0 ,       0 ,     0 ,     1   ]])
		
		self.world2img = np.mat([ [ cols,   0 ,  0 ,  0.5*cols  ],
		                          [   0 , rows,  0 ,  0.5*rows  ],
								  [   0 ,   0 ,  1 ,   0        ],
								  [   0 ,   0 ,  0 ,   1        ]])
		

		# Define the camera position
		self.Po[0,0] = float(root.find('camera/position').attrib.get('x'))
		self.Po[1,0] = float(root.find('camera/position').attrib.get('y'))
		self.Po[2,0] = float(root.find('camera/position').attrib.get('z'))
		
		self.rotation_angle   = float(root.find('camera/rotation').attrib.get('angle'))
		self.rotation_axis[0] = float(root.find('camera/rotation').attrib.get('axis_x'))
		self.rotation_axis[1] = float(root.find('camera/rotation').attrib.get('axis_y'))
		self.rotation_axis[2] = float(root.find('camera/rotation').attrib.get('axis_z'))
		
		self.rotation = Quaternion( self.rotation_angle, self.rotation_axis).get_rotation_matrix()
	
		# Define the Vf vector
		self.Vf_in = self.rotation * np.mat([[0],[0],[-1],[0]])

	def __str__(self):
		strout  = 'Configuration Filename\n'
		strout += '-Image Filename: ' + self.image_filename + '\n'
		strout += '-Image Size    : ' + str(len(image[0])) + ', ' + str(len(image)) + '\n'
		strout += '-Focal Length  : ' + str(self.f) + '\n'
		strout += '-Cam Position  : ' + str(self.Po[0,0]) + ', ' + str(self.Po[1,0]) + ', ' + str(self.Po[2,0]) + '\n'
		strout += '-Cam Rotation Angle: ' + str(self.rotation_angle) + ', axis: ( ' + str(self.rotation_axis[0]) + ', ' + str(self.rotation_axis[1]) + ', ' + str(self.rotation_axis[2]) + ' )\n'
		strout += '\n'
		strout += '-Viewing Vector: ' + str(self.Vf_in[0]) + ', ' + str(self.Vf_in[1]) + ', ' + str(self.Vf_in[2]) + '\n'

		return strout


def main():
	
	# Open up the config file and read the image parameters
	options = Options('OrthoProjector.xml')

	# Print the configuration to screen
	print options
	
	# Create the output image
	corrected = np.zeros( (rows, cols), np.uint8 )


	# Compute the translation required to center the camera from the original to the stabilized
	N = np.mat([[0],[0],[1],[0]])
	P1 = options.Po
	P2 = options.Po + options.Vf_in
	P3 = np.mat([[0],[0],[0], [1]])
	PT = np.dot( N.T, P3-P1)[0,0]
	u  = PT/(np.dot( N.T, (P2 - P1)))[0,0]
	stare_point = P1 + u*(P2-P1)
	nadir_point = np.mat([[options.Po[0,0]], [options.Po[1,0]], [0], [1]])

	required_translation = stare_point - nadir_point

	# This is the final position of the camera as shown in the final image
	Pf = options.Po + required_translation

	# Start iterating through the image
	spacer = np.mat([[0],[0],[0],[1]])
	cnt = 0
	hx = cols/2.0
	hy = rows/2.0
	for x in xrange(0, cols):
		for y in xrange(0, rows):
			
			#######################################################################
			# STEP 1: Find the actual coordinate of the pixel in the final image  #
			#######################################################################
			# Convert the image coordinate to world coordinates
			iCoord = options.img2world * np.mat([[x],[y],[0],[1]]) + (options.f*options.Vf_out) + Pf - spacer

			# Compute the vector which starts at the origin and radiates to the pixel location
			V_view = iCoord - Pf
			
			# Compute intersection between point and ground
			N = np.mat([[0],[0],[1],[0]])    		 # This is the normal to the ground plane "straight up"
			P1 = Pf                                  # This is the origin of the final camera position
			P3 = np.mat([[0],[0],[0],[1]])           # This is some point on the earth which is not on the line
			
			# Divide the projection of the earth normal with the camera to earth line against the projection of the earth normal with the viewing vector
			# This will give you the scale of the vector against V_view
			u = (np.dot( N.T, P3-P1)[0,0])/((np.dot( N.T, V_view))[0,0])
			
			# We need to now find this point in the original image
			ground_point = P1 + u*(V_view)
		
			########################################################################################
			# STEP 2: Relate the physical coordinate with the pixel location in the original image #
			########################################################################################
			
			# Compute the normalized vector between the actual point and the camera origin
			V_view = (ground_point - options.Po)
			V_view = V_view / np.linalg.norm(V_view)

			# Compute the camera coordinate from that in the local camera space
			localCoord = V_view * options.f + spacer


			# Relate that to the original camera image plane
			N = options.Vf_in		# Normal Vector of the Image Plane
			P1 = spacer				# Origin of the camera
			P2 = localCoord			# Point on the camera
			P3 = options.rotation * np.mat([[-0.5],[-0.5],[-options.f],[1]]) # Point on the plane
			u = (np.dot( N.T, P3-P1)[0,0])/((np.dot( N.T, P2-P1))[0,0])

			# We need to now find this point in the original image
			imgPoint = ( P1 + u*(P2-P1))

			# Compute the difference between the points
			diff = options.world2img * ( imgPoint - N )

			# Make sure coordinate fits inside the image
			if int(diff[0,0])  >= 0   and int(diff[1,0]) >= 0 and int(diff[0,0]) < cols and int(diff[1,0]) < rows:
				corrected[y,x] = image[int(diff[1,0]),  int(diff[0,0])]

			
			# A counter to keep my sanity
			if cnt % 10000 == 0:
				sys.stdout.write('x')
				sys.stdout.flush()

			cnt+= 1
			
		#cv2.imshow('Original', image)
		#cv2.imshow('Corrected', corrected)
		#cv.WaitKey(0)

	
	cv.NamedWindow('Corrected')
	cv.NamedWindow('Original')
	cv2.imshow('Original',  image)
	cv2.imwrite('original.jpg', image)
	cv2.imshow('Corrected', corrected)
	cv2.imwrite('corrected.jpg', corrected)
	cv.WaitKey(0)


if __name__ == '__main__':
	main()

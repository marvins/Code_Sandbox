#! /usr/bin/env python

import sys, os, errno, xml.etree.ElementTree as xml, datetime, time

months  = ['JAN','FEB','MAR','APR','MAY','JUN','JUL','AUG','SEP','OCT','NOV','DEC']
cameras = ['cam1','cam2','cam3','cam4','cam5']


###########################################
#			Configuration Options		  #
###########################################
class ConfigOptions:
	"""
	Configuration Options for the Calibration Collection Program
	"""
	
	root = []
	config_filename = 'mosaic_analyzer.xml'
	
	debug_level      = 0
	output_code      = 0
	logging_state    = False
	log_location     = '.mosaic_analyzer.log'

	input_directory = 'mosaic'
	output_filename = 'output.csv'

	#-     Constructor    -#
	def __init__( self, args ):
		"""
		Default Constructor for the Configuration Options 
		"""
		
		# Load the ini file
		for arg in args:
			if arg[:6] == '-file=':
				self.config_filename = arg[6:]
			else:
				raise Exception('Invalid option. Not supported')


		# make sure it exists
		if os.path.exists( self.config_filename ) == False:
			raise Exception('Config file: '+self.config_filename+' does not exist')

		
		###############################
		#      LOAD CONFIG FILE       #
		###############################
		# open root node
		self.root = xml.parse(self.config_filename)

		# Grab the important configuration options
		self.debug_level     = int(self.root.find('debug_level').attrib.get('value'))
		self.logging_state   = bool(self.root.find('logging').attrib.get('state'))
		self.log_location    = self.root.find('logging').attrib.get('location')
		self.input_directory = self.root.find('input_directory').attrib.get('value')
		self.output_filename = self.root.find('output_file').attrib.get('value')

	
	################################################
	#-      Print Instructions On How To Use 	  -#
	################################################
	def usage( self ):
		"""
		Usage function for the program
		"""

		print sys.argv[0] + ' [options]'

	def __str__( self ):
		"""
		Print function for the Command Line option class
		"""

		strout  = 'Command-Line Options ' + '\n'

	###########################################
	#        Default Error Function			  #
	###########################################
	def error_kill( self, msg, output_code ):
		print(msg)
		self.usage()
		sys.exit(output_code)


#####################################
#             TACID Parser			#
#####################################
class TACID:
	input_string     = []
	raw_input        = []
	acquisition_date = []
	program_code     = []
	sortie_number    = []
	scene_number     = []
	producer_code    = []
	product_number	 = []
	project_code	 = []
	replay		     = []
	producer_sn		 = []
	production_datim = []
	failed           = False

	def __init__(self, imgname ):
		
		try:

			instr = os.path.basename(imgname)
			self.input_string = instr
			self.raw_input = imgname
	
			# set the date
			day  = int(instr[:2])
			mon  = instr[2:5]
			year = int(instr[5:7]) + 2000
			for x in xrange(len(months)):
				if months[x] == mon:
					mon = x+1
					break
			t = time.mktime((year,mon,day, 0, 0, 0, 0, 0, -1))
			self.acquisition_date = time.gmtime(t)
			instr = instr[7:]
	
			# get program_code
			self.program_code = instr[0:2]
			instr = instr[2:]

			# get sortie_number
			self.sortie_number = instr[0:2]
			instr = instr[2:]
	
			# get scene number
			self.scene_number = int(instr[:5])
			instr = instr[5:]

			# get DoD producer code
			self.producer_code = instr[:2]
			instr = instr[2:]

			# get product number
			self.product_number = instr[:6]
			instr = instr[6:]
		
			# get the NGA Project Code
			self.project_code = instr[:2]
			instr = instr[2:]

			# get the replay (reprocessed or retransmitted state flag)
			self.replay = instr[:3]
			instr = instr[3:]

			# get the producer serial number
			self.producer_sn = instr[:3]
			instr = instr[3:]
	
			# production datim
			value = '0x' + instr[:8]
			self.production_datim = int(value, 16)
			instr = instr[8:]
		
		except ValueError as ve:
			self.failed = True

		finally:
			return


	def __str__(self):
		output  = 'input string     : ' + self.input_string			 + '\n'
		output += 'acquisition date : ' + str(self.acquisition_date) + '\n'
		output += 'program code     : ' + str(self.program_code)     + '\n'
		output += 'sortie number    : ' + str(self.sortie_number)	 + '\n'
		output += 'scene number     : ' + str(self.scene_number)	 + '\n'
		output += 'producer code    : ' + str(self.producer_code)	 + '\n'
		output += 'product number   : ' + str(self.product_number)   + '\n'
		output += 'project code     : ' + str(self.project_code)     + '\n'
		output += 'replay           : ' + str(self.replay)			 + '\n'
		output += 'producer_sn      : ' + str(self.producer_sn)		 + '\n'
		output += 'production_datim : ' + str(self.production_datim) + '\n'

		return output
	
	################################
	#-     Get the Camera Type    -#
	################################
	def getCameraType( self ):
		"""
		This function will return the first three characters from the product number.
		If the camera is EO, it should return GEO. If the camera is IR, it should 
		return GIR.  All other options will throw exceptions until adequetly debugged.
		"""
		
		if len( self.product_number ) != 6:
			raise Exception('ERROR: Product Number for Image is Not 6 characters')

		ctype = self.product_number[:3]
		ctype = ctype[1:]
		
		if ctype != 'EO' and ctype != 'IR':
			raise Exception('ERROR: Product Number does not contain an adequate Camera Type')

		return ctype


def nodesEqual( nodeA, nodeB ):
	
	if os.path.basename(nodeA.name) == os.path.basename(nodeB.name):
		if nodeA.type == nodeB.type:
			return True
	
	return False

def isValidTACID( node ):

	if len(os.path.basename(node.name)) != 44:
		return False
	
	return True



#--------------------------------------------#
#-      Sort the image list by history		-#
#--------------------------------------------#
def sort_image_list( root_dir ):

	# set x to the starting point
	x = 1
	
	# iterate throught the entire list
	while x < (len(root_dir)):
		
		# create the test point
		item    = root_dir[x][0]
		item_pn = item.producer_sn
		item_sn = item.scene_number

		# iterate through the items before the test point
		y = 0
		i = -1
		FLG = False
		
		while y < x:
	    	
			# set anchor at where the test point become less than
			if item_pn < root_dir[y][0].producer_sn:
				i = y
				break
			elif item_pn == root_dir[y][0].producer_sn:
				if item_sn < root_dir[y][0].scene_number:
					i = y
					break
				if item_sn == root_dir[y][0].scene_number:
					root_dir[y] = root_dir[y] + root_dir[x]
					root_dir.pop(x)
					FLG = True
					break
			
			y += 1
			

		if FLG == True :
			continue
		
		if y < x:
			temp = root_dir[x]

			z = x
			while z > i:
				root_dir[z] = root_dir[z-1]
				z = z - 1
			root_dir[i] = temp
		
		x += 1
		



#------------------------------------------------------------#
#-   				Build Image List						-#
#------------------------------------------------------------#
def build_image_list( root_dir, options ):
	"""
	This function takes an image directory and searches for all images matching a required pattern. 
	If they exist, it will parse the image name and add the image to the list for that image directory.
	"""

	# create an empty list
	images = []

	# find the contents of the directory
	contents = os.listdir( root_dir )

	# iterate through each item to either add or enter
	for item in contents:
		
		# check if item is file or directory
		if os.path.isdir( root_dir + '/' + item ) == True:

			# enter directory recursively
			images = images + build_image_list( root_dir + '/' + item, options )

		# otherwise, check if file matches
		elif os.path.isfile( root_dir + '/' + item ) == True:
			
			# check if the extension is a nitf
			if os.path.splitext( item )[1] == ".ntf" or os.path.splitext( item )[1] == ".NTF":
			
				IMG = TACID(root_dir + '/' + item)
				
				# ensure that the image is valid
				if IMG.failed == False:
					images = images +  [[IMG]]
				

		else:
			raise Exception("ERROR: must be file or directory")

		
	return images



##################################################
#-					Main Driver					-#
##################################################
def main():
	
	# Parse command-line options
	options = ConfigOptions( sys.argv[1:] )

	# we need to build an array which contains the list of files for each camera folder
	image_list = []
	output_list = []
	
	# create image list
	image_list = build_image_list( options.input_directory, options )

	# sort the list
	sort_image_list( image_list )
	

	# iterate over the list and pull out some relevant metrics
	data = []
	
	basetime = -1
	count = 1
	
	# create csv output filename
	csv_file = open(options.output_filename, 'w')
	
	for img in image_list:
		for idx in img:
			
			yr = 0
			mn = 0
			dy = 0
			hr = 0
			mi = 0
			sc = 0
			ms = 0
			frame_number = 0
			timestamp = ''

			# Grab the output of the program
			console = os.popen('gdalinfo ' + idx.raw_input).readlines()
			
			# iterate over every line
			for line in console:
				
				# check for the comment field
				if 'NITF_IMAGE_COMMENTS' in line:
					
					#split and strip the values
					data = line.split(' ')
					for i in xrange(0, len(data)-1):
						
						if 'FRAME' in data[i] and 'NUMBER:' in data[i+1]:
							frame_number = int(data[i+2])

						if 'TIMESTAMP:' in data[i]:
							
							timestamp = data[i+1]
							yr=int(data[i+1][0:4])
							mn=int(data[i+1][4:6])
							dy=int(data[i+1][6:8])
							hr=int(data[i+1][8:10])
							mi=int(data[i+1][10:12])
							sc=int(data[i+1][12:14])
							ms=float(data[i+1][14:])
			
			# create time stamp object
			date_obj = datetime.datetime( yr, mn, dy, hr, mi, sc, int(ms))

			unixtime = time.mktime(date_obj.timetuple()) + ms

			# initialize base time
			if basetime == -1:
				basetime = unixtime
			
			# compute the time difference
			time_diff = unixtime - basetime

			# add to list
			output_string = idx.input_string + ', ' + str(frame_number) + ', ' + str(timestamp) + ', ' + str(time_diff)
			csv_file.write( output_string + '\n')

			print 'image ' + str(count) + ' of ' + str(len(image_list)) + ' -> ' + output_string
			count += 1

			basetime = unixtime


	# close up shop	
	csv_file.close()
	sys.exit(options.output_code)

if __name__ == "__main__":
	main()

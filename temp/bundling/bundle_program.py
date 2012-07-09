#! /usr/bin/env python

import sys, os, time, tarfile, zipfile

months  = ['JAN','FEB','MAR','APR','MAY','JUN','JUL','AUG','SEP','OCT','NOV','DEC']
cameras = ['cam1','cam2','cam3','cam4','cam5']


###########################################
#			Configuration Options		  #
###########################################
class ConfigOptions:
	"""
	Configuration Options for the Calibration Collection Program
	"""
	
	# List all of the command line options and assign initial values
	camera_type      = '_NONE_'
	camera_set       = False
	
	input_base 		 = '.'
	input_path  	 = '_NONE_'
	output_base 	 = '.'
	output_path		 = 'bundle'
	
	num_bundles      = 2
	debug_level      = 0
	num_eo_camera_directories = -1
	num_ir_camera_directories = -1
	num_eo_images_per_cam = 1
	num_ir_images_per_cam = 1
	compression_type = '_NONE_'

	#-     Constructor    -#
	def __init__( self ):
		"""
		Default Constructor for the Configuration Options 
		"""
		
		# Load the ini file
		inifilename = os.path.split(sys.argv[0])[0] + '/bundle_program.cfg'

		# make sure it exists
		if os.path.exists( inifilename ) == False:
			print 'Error: could not find config file'
			sys.exit(-1)
		
		###############################
		#      LOAD CONFIG FILE       #
		###############################
		inifile=open(inifilename, 'r')
		inidata= inifile.readlines()
		
		for str in inidata:
			
			# Strip Whitespace from Line			
			str = str.strip()

			# Ignore Empty Lines
			if   len(str) <= 0:
				pass

			# Ignore comments
			elif str[0] == '#':
				pass

			else:
				
				# Split line into item & object
				hdr, dta = str.split('=')

				# Strip whitespace from item and object
				hdr = hdr.strip()
				dta = dta.strip()

				# Parse the Prefix Directory
				if   hdr == 'input_base':
					self.input_base = dta

				elif hdr == 'output_base':
					if dta[-1] == '/':
						dta = dta[:-1]
					self.output_base = dta

				elif hdr == 'output_path':
					self.output_path = dta
				
				elif hdr == 'debug_level':
					self.debug_level = int(dta)
				
				elif hdr == 'num_eo_dir':
					self.num_eo_camera_directories = int(dta)
				
				elif hdr == 'num_ir_dir':
					self.num_ir_camera_directories = int(dta)
				
				elif hdr == 'eo_images_per_camera':
					self.num_eo_images_per_cam = int(dta)
				
				elif hdr == 'ir_images_per_camera':
					self.num_ir_images_per_cam = int(dta)
				
				elif hdr == 'compression_type':
					if dta[0] != '.':
						dta = '.' + dta
					self.compression_type = dta
				
				elif hdr == 'num_bundles':
					self.num_bundles = int(dta)

				elif hdr == 'camera_type':
					if self.camera_set == True:
						print 'Conflicting messages, camera_type already set, ignoring'
					else:
						self.camera_type = dta
						self.camera_set = True

		##########################################
		#       PARSE COMMAND-LINE OPTIONS       #
		##########################################
		# begin sweeping over items, looking for matching parameters
		command_args = sys.argv[1:]
		while( len(command_args) > 0 ):
			
			# input camera directory
			if command_args[0][:12] == '-input_path=':
				self.input_path = command_args[0][12:]

				if self.input_path[-1] == '/':
					self.input_path = self.input_path[:-1]
				
				command_args = command_args[1:]
			
			# prefix directory
			elif command_args[0][:12] == '-input_base=':
				self.input_base = command_args[12:]
				command_args = command_args[1:]
			
			# output camera directory
			elif command_args[0][:13] == '-output_base=':
				self.output_base = command_args[0][13:]
				command_args = command_args[1:]
			
			# output camera bundle
			elif command_args[0][:13] == '-output_path=':
				self.output_path = command_args[0][13:]
				command_args = command_args[1:]

			# debugging level
			elif command_args[0][:13] == '-debug_level=':
				self.debug_level = int(command_args[0][13:])
				command_args = command_args[1:]


			# input number of camera bundles
			elif command_args[0][:13] == '-num_bundles=':
				self.num_bundles   = int(command_args[0][13:])
				command_args = command_args[1:]
			
			# type of camera (EO or IR)
			elif command_args[0][:10] == '-cam_type=':
				
				if command_args[0][10:] == 'EO':
					self.camera_type = 'EO'
					self.camera_set  = True

				elif command_args[0][10:] == 'IR':
					self.camera_type = 'IR'
					self.camera_set  = True
				
				else:
					raise Exception('ERROR: unknown camera type: ' + command_args[0][10:])

				command_args = command_args[1:]
			
			# help
			elif command_args[0][:6] == '--help' or command_args[0][:5] == '-help' or command_args[0][:4] == 'help':
				self.usage()
				sys.exit(-1)

			# throw an exception if an option is not recognized
			else:
				print('ERROR: Unknown parameter: ' + command_args[0]);
				self.usage()
				sys.exit(-1)
	
		# do some management
		if len(self.input_path) >= 1 and self.input_path[0] == '/':
			self.input_base = '/'
			self.input_path = self.input_path[1:]

		if len(self.output_path) >= 1 and self.output_path[0] == '/':
			self.output_base = '/'
			self.output_path      = self.output_path[1:]
		
		if len(self.input_base) >= 1 and self.input_base[-1] != '/':
			self.input_base += '/'

		if len(self.output_base) >= 1 and self.output_base[-1] != '/':
			self.output_base += '/'

		# make sure everything we need is present
		if self.input_path == '_NONE_':
			print('ERROR: input camera directory must be defined')
			self.usage()
			sys.exit(-1)

		elif self.compression_type != '.zip':
			self.error_kill( 'ERROR: Invalid compression format, must be .zip')

		elif self.camera_set == False:
			self.error_kill( 'ERROR: camera_type must be set in either .cfg config file or command-line options', -1)	
		
		if self.debug_level >= 1:
			print('Printing Current Configuration Options')
			print self
			print('')
			print('')
			
			if self.debug_level >= 2:
				raw_input("HOLDING")

	
	################################################
	#-      Print Instructions On How To Use 	  -#
	################################################
	def usage( self ):
		"""
		Usage function for the program
		"""

		print sys.argv[0] + ' [options]'
		print ''
		print '   options:'
		print '       -input_base=<directory> 		- Prefix of where the directory will be'
		print '       -input_path=<input file>			- Directory you want to bundle'
		print '       -output_path=<output file>  			- (output will be tar.gz so don\'t add it to the string)'
		print '       -num_bundles=< integer >              - number of desired image bundles'
		print '       -cam_type=<EO or IR>'
		print '       -debug_level=<0 default>  [0 - only fatal, 1 - everything]'
		print ''
		print '   .ini config options'
		print '       input_base=<value>   -- Where you want to start with the input path'
		print '       input_path=<value>    -- Name of input directory to bundle'
		print '       output_base=<value>   -- Where to place the results with respect to the input'
		print '     '
		print ''
		print '   Notes: '
		print '      - Prefix Directory and Output Directory will default to .'
		print '      - If you make the input directory absolute, the prefix will be ignored'
		print '      - If you make the output file absolute, the output_base will be ignored'

	def __str__( self ):
		"""
		Print function for the Command Line option class
		"""

		strout  = 'Command-Line Options ' + '\n'
		strout += '     Prefix Directory   : ' + self.input_base + '\n'
		strout += '     Input Directory    : ' + self.input_path  + '\n'
		strout += '     Output Directory   : ' + self.output_base + '\n'
		strout += '     Output File        : ' + self.output_path      + '\n'
		strout += '     Number Bundles     : ' + str(self.num_bundles) + '\n'
		strout += '     Camera Type        : ' + self.camera_type      + '\n'
		strout += '     Camera Set         : ' + str(self.camera_set)  + '\n'
		strout += '     Debug Level        : ' + str(self.debug_level) + '\n'
		strout += '     Num EO Directories : ' + str(self.num_eo_camera_directories) + '\n'
		strout += '     Num EO Img Per Cam : ' + str(self.num_eo_images_per_cam) + '\n'
		strout += '     Num IR Directories : ' + str(self.num_ir_camera_directories) + '\n'
		strout += '     Num IR Img Per Cam : ' + str(self.num_ir_images_per_cam) + '\n'
		strout += '\n'
		strout += '     Input Dir Path     : ' + self.input_base + self.input_path + '\n'
		strout += '     Output file will be: ' + self.output_base + self.output_path + self.compression_type + '\n'
		return strout

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


	def __init__(self, imgname ):
	
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



#------------------------------------------------#
#-    			Basic Error Checking  			-#
#------------------------------------------------#
def validityCheckPre( directory ):
	"""
	This function does a basic check to make sure that the directory we have passed into the program
	actually exists and furthermore that the directory is actually a directory. 
	"""

	# Make sure that the directory we are searching for exists
	if os.path.exists( directory ) == False:
		print 'Error: ' + directory + ' does not exist'
		sys.exit(-1)

	# make sure that it is a directory
	if os.path.isdir(directory) == False:
		print 'Error: ' + directory + ' is not a directory '
		sys.exit(-1)
	
	return True


def validityCheckPost( camera_roots, options ):
	"""
	This function checks that the root camera directories we have located conform
	to the requirements of the specific camera type. The primary assumption made 
	here is that the camera type has been set.
	"""
	
	# sort camera roots
	camera_roots.sort()
	
	if options.debug_level >= 2:
		print 'detected directories pre validation'
		for img in camera_roots:
			print img
		raw_input('hold')
		print ''


	# Test IR Cameras
	if 	options.camera_type == 'IR':
		
		if len(camera_roots) < options.num_ir_camera_directories:
			print 'ERROR: directory does not have enough cam directories to satisfy expected requirement'
			sys.exit(-1)
		
		elif len(camera_roots) > options.num_ir_camera_directories:
			camera_roots = camera_roots[:options.num_ir_camera_directories]
	
	# Test EO Cameras
	elif options.camera_type == 'EO':
		if len(camera_roots) < options.num_eo_camera_directories:
			print 'ERROR: directory does not have enough cam directories to satisfy expected requirement'
			sys.exit(-1)
		elif len(camera_roots) > options.num_eo_camera_directories:
			camera_roots = camera_roots[:options.num_eo_camera_directories]
	else:
		raise Exception('unexpected camera type: ' + options.camera_type)

	# make sure we got something useful
	if camera_roots == None or len(camera_roots) <= 0:
		raise Exception('Error: directory contains no camera directories')

	if options.debug_level >= 2:
		print 'detected directories post validation'
		for img in camera_roots:
			print img
		raw_input('hold')
		print ''


	return camera_roots


#------------------------------------------------#
#-    Find Camera Directory						-#
#-												-#
#-    Look for the baseline camera directory    -#
#------------------------------------------------#
def find_camera_directory( directory ):
	


	# extract the contents of the directory
	contents = os.listdir(directory);

	# remove all non-directories
	directories = [elem for elem in contents if os.path.isdir(directory+'/'+elem) == True]
	
	# search to see if cam directories are present
	dir_stack = []
	for d in directories:
		try:
			idx = cameras.index(d) 
			dir_stack.append(directory+'/'+d)
		except ValueError:
			pass
	
	# if directory stack is empty or does not contain a sufficient number of cameras, then
	# we need to step into any directories in the current location
	if len(dir_stack) <= 0:
	
		# if there are no more directories, then we need to return a null list
		if directories == []:
			return []
		# Test each remaining directory
		else:
			for d in directories:
				dir_stack = find_camera_directory( directory + '/' + d )
				
				if dir_stack != []:
					return dir_stack
				else:
					pass

	# Recursive Exit Condition		
	else:
		return dir_stack




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
				
				# ensure the image has the proper camera type
				ctype = IMG.getCameraType()

				if ctype == options.camera_type:
					images = images +  [[IMG]]
				
				else:
					pass

		else:
			raise Exception("ERROR: must be file or directory")

		
	return images


#------------------------------------------------------------#
#-        			Image Tuple Match      					-#
#------------------------------------------------------------#
def image_tuple_match( cam_lists, options ):
	""" 
	This function determines if the top items in each camera array are from the same 
	time step.  If they are, then it returns true.  Otherwise, it returns false.
	"""

	# iterate over each camera file listing and see if the top of each list matches
	top_item = cam_lists[0][0][0]
	
	for lst in cam_lists:

		# make sure the required number of images exist per frame
		if options.camera_type == 'EO':
			if len(lst[0]) != options.num_eo_images_per_cam:
				return False
		elif options.camera_type == 'IR':
			if len(lst[0]) != options.num_ir_images_per_cam:
				return False
		else:
			raise Exception('Unknown camera type')

		# check scene number
		if top_item.scene_number != lst[0][0].scene_number:
			return False

		# check product number
		if top_item.product_number[:3] != lst[0][0].product_number[:3]:
			return False

		# check producer serial number
		if top_item.producer_sn != lst[0][0].producer_sn:
			return False
	
	return True


#--------------------------------------------------------#
#-					Pop Earliest Image					-#
#--------------------------------------------------------#
def pop_earliest_image( cam_lists ):
	"""
	Remove the earliest image from the list of image lists.  This 
	is basically just popping the top item from a list of stacks.
	"""

	top_item = cam_lists[0][0][0]
	top_sn   = cam_lists[0][0][0].scene_number
	top_pn   = cam_lists[0][0][0].producer_sn
	top_idx  = 0

	for idx in range( 1, len(cam_lists)):

		# compare the top item with the current list head item
		if top_pn == cam_lists[idx][0][0].producer_sn:
			
			if top_sn <= cam_lists[idx][0][0].scene_number:
				continue
			elif top_sn > cam_lists[idx][0][0].scene_number:
				top_item = cam_lists[idx][0][0]
				top_sn   = cam_lists[idx][0][0].scene_number
				top_pn   = cam_lists[idx][0][0].producer_sn
				top_idx  = idx
			else:
				raise Exception("ERROR")
		
		elif top_pn < cam_lists[idx][0][0].producer_sn:
			continue
		
		elif top_pn > cam_lists[idx][0][0].producer_sn:
			top_item = cam_lists[idx][0][0]
			top_sn   = cam_lists[idx][0][0].scene_number
			top_pn   = cam_lists[idx][0][0].producer_sn
			top_idx  = idx
			continue

		else:
			raise Exception("ERROR")

	# remove oldest image
	cam_lists[top_idx].pop(0)


######################################
#-      Prune The Camera List       -#
######################################
def prune_camera_list( cam_lists, options ):
	""" 
	Prune that camera list to remove all images which don't match
	between each required camera directory of don't have enough images
	per camera step.
	"""

	# find the length of the shortest camera directory
	min_len = len( cam_lists[0] )
	for x in range( 1, len( cam_lists )):
		if min_len > len( cam_lists[x] ):
			min_len = len( cam_lists[x] )
	
	# iterate through each list, looking for matching images
	breakNow = False

	output = []
	
	while True:
		
		# break the loop if a camera list is empty. That means the rest of the lists are junk
		for x in range( 0, len( cam_lists )):
			if len( cam_lists[x] ) <= 0:
				breakNow = True
				break

		# If a list is empty or another mode request a break, then exit the while loop
		if breakNow == True:
			break

		# if all cam top images don't match, then pop the youngest until they do
		if  image_tuple_match( cam_lists, options ) == False:
			pop_earliest_image( cam_lists )
		
		# If they do match, then add it to the output list
		else:
			st = []
			for x in range( 0, len(cam_lists)):
				st = st + cam_lists[x][0]
				cam_lists[x] = cam_lists[x][1:]
			
			output.append(st)

	return output


##################################################
#-					Main Driver					-#
##################################################
def main():

	# Parse command-line options
	options = ConfigOptions()
	
	# make sure the data is in an intelligible format
	validityCheckPre( options.input_base + options.input_path);

	# do recursive traversal to find the camera directory location
	camera_roots = find_camera_directory( options.input_base + options.input_path )
	
	# make sure that we have an expected number of camera directories
	camera_roots = validityCheckPost( camera_roots, options );


	# we need to build an array which contains the list of files for each camera folder
	cam_lists = []
	
	if options.debug_level >= 1:
		print 'building camera array'
	
	for x in range( 0, len(camera_roots)):
		
		# create image list
		cam_lists.append( build_image_list( camera_roots[x], options ))
		
		sort_image_list( cam_lists[x] )
		
		if options.debug_level >= 2:
			print 'images for list ' + str(x) + ' in directory ' + camera_roots[x]
			for y in range( 0, len(cam_lists[x])):
				print 'scene number: ' + str(cam_lists[x][y][0].scene_number) + '   with size: ' + str(len(cam_lists[x][y]))
			raw_input('hold')
			print ''

	# next, we need to ensure that each camera folder contains only matching image pairs
	if options.debug_level >= 1:
		print 'pruning the list'
	
	image_tuples = prune_camera_list( cam_lists, options )
	
	
	if options.debug_level >= 2:
		print 'printing the pruned tree'
		for x in range( 0, len(image_tuples)):
			print '   bundle: ' + str(x) + '  --> len ' + str(len(image_tuples[x]))
		raw_input('hold')
		print ''

	#  A potential problem exists where we may not have enough image sets to satisfy the 
	# request given in number of bundles variable.  This means that we must bundle what we 
	# and send an error back notifying that we only sent 'N' values
	options.num_bundles = min( options.num_bundles, len(image_tuples))


	# now that we have a list of images, we need to start dividing image sets
	bundle_step = len(image_tuples) / options.num_bundles	

	# create output data
	zf = zipfile.ZipFile( options.output_base + options.output_path + options.compression_type, 'w')
	
	if options.debug_level >= 1:
		print 'writing files to : ' + options.output_base + options.output_path + options.compression_type

	# create current position pointer
	current_pos = 0
	for x in range(0, options.num_bundles):
		
		# increment position
		current_pos = bundle_step * x
		for y in range( 0, len(image_tuples[current_pos])):
			
			# add file to tarball
			if options.debug_level >= 2:
				print image_tuples[current_pos][y].raw_input
			zf.write( image_tuples[current_pos][y].raw_input, 'bundles/bundle'+str(x)+'/'+image_tuples[current_pos][y].input_string)

	zf.close()
	

if __name__ == "__main__":
	main()

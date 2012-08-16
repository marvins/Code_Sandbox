#! /usr/bin/env python

import sys, os, time, tarfile, zipfile, paramiko, xml.etree.ElementTree as xml

months  = ['JAN','FEB','MAR','APR','MAY','JUN','JUL','AUG','SEP','OCT','NOV','DEC']
cameras = ['cam1','cam2','cam3','cam4','cam5']

NEWLINE = '\n'
log = []


#################################
#         Program Logger 		#
#################################
class Logger:
	"""
	Class for managing the Logs for the program
	"""


	log_file = []

	# LEVELS
	MAJOR   = 0
	MINOR   = 1
	WARNING = 2
	INFO    = 3
	HEADING = 4

	# Internal parameters
	log_state     = False
	log_location  = ".bundle_program.log"
	debug_state   = 0
	logfile = []


	def __init__( self, debug_state, log_state, log_location ):
		
		self.debug_state  = debug_state
		self.log_state    = log_state
		self.log_location = log_location
		
		if  self.log_state == True:
			self.open()

	
	def open(self):
		self.logfile = open( self.log_location, 'w')
	
	def close(self):
		self.logfile.close()

	def write( self, level, message ):
		
		# create message
		output = self.create_log_message( level, message)

		# major events
		if ( level == 0 and self.debug_state >= 1 ) or ( level > 0 and self.debug_state >= 2 ):
            
			# write to console
			print( output[:-(len(NEWLINE))] )

		# write error to log if debug level and log file are allowed
		if self.log_state == True:
			self.logfile.write( self.create_log_message(level, message))
			


	def create_log_message( self, level, message):
		
		if level == self.MAJOR:
			return '[ MAJOR   ]: ' + message + NEWLINE
		elif level == self.MINOR:
			return '[ MINOR   ]: ' + message + NEWLINE
		elif level == self.WARNING:
			return '[ WARNING ]: ' + message + NEWLINE
		elif level == self.HEADING:
			return message + NEWLINE
		else:
			return '[ INFO    ]: ' + message + NEWLINE




# Utility function for testing xml values
def xmlValidateAndLoad( root, nodeName, valueName, valueType, datatype ):
	
	variable=[]
	if root.find(nodeName) == None:
		return [False,None] 

	if valueType == 'ATTRIBUTE':
		if root.find(nodeName).attrib.get(valueName) != None:
			if datatype == 'STRING':
				variable = root.find(nodeName).attrib.get(valueName)
			elif datatype == 'BOOL':
				variable = bool(int(root.find(nodeName).attrib.get(valueName)))
			elif datatype == 'INT':
				variable = int(root.find(nodeName).attrib.get(valueName))
			return [True,variable]
	
	return [False,None]


###########################################
#			Configuration Options		  #
###########################################
class ConfigOptions:
	"""
	Configuration Options for the Calibration Collection Program
	"""
	
	# default configuration information
	config_filename = "bundle_program.xml"

	# some useful stuff for output
	debug_level = 0
	output_code = 0

	# logging information
	log_state    = False
	log_location = "bundle_program.log"
	
	# SSH flags
	ssh_state = False
	ssh_hostname  = ''
	ssh_username  = ''
	ssh_password  = ''

	# General required parameters
	number_bundles   = 3
	compression_type = "zip"
	camera_type      = "EO"
	gs_increment     = 1
	
	# Path Information
	input_base 		 = '.'
	input_path  	 = '_NONE_'
	output_base 	 = '.'
	output_path		 = 'bundle'
	
	# GS1 Specific
	num_eo_camera_directories = -1
	num_ir_camera_directories = -1
	num_eo_images_per_cam = 1
	num_ir_images_per_cam = 1
	
	# GS2 Specific
	number_slices   = 1
	number_collects = 1
	
	eo_fism_count   = 200
	if_fism_count   = 200

	

	#-     Constructor    -#
	def __init__( self, args ):
		"""
		Default Constructor for the Configuration Options 
		"""
			
		# Due to the nature of the process, we want to process the config file first, then overwrite them
		# with any command-line options.  However we need to search for a config file override, so we will
		# search for that first
		for arg in args:
			if '-file=' in arg[:6]:
				self.config_filename = arg[6:]
		
		# Let's ensure the config file exists
		if os.path.exists(self.config_filename) == False:
			raise Exception('Config file: ' + self.config_filename + ' does not exist')

		
		###############################
		#      LOAD CONFIG FILE       #
		###############################
		root = xml.parse(self.config_filename)
		
		# debugging information
		if xmlValidateAndLoad( root, 'debug_level', 'value', 'ATTRIBUTE', 'INT')[0] == True:
			self.debug_level = xmlValidateAndLoad(root,'debug_level','value','ATTRIBUTE','INT')[1]


		# logging information
		if xmlValidateAndLoad( root, 'log',    'state', 'ATTRIBUTE',   'BOOL')[0] == True:
			self.log_state = xmlValidateAndLoad( root, 'log','state','ATTRIBUTE','BOOL')[1]

		if xmlValidateAndLoad( root, 'log', 'location', 'ATTRIBUTE', 'STRING')[0] == True:
			self.log_location = xmlValidateAndLoad(root,'log','location','ATTRIBUTE','STRING')[1]
		
	 	# ssh information
		if xmlValidateAndLoad( root, 'ssh', 'state', 'ATTRIBUTE', 'BOOL')[0] == True:
			self.ssh_state = xmlValidateAndLoad(root,'ssh','state','ATTRIBUTE','BOOL')[1]
		
		if xmlValidateAndLoad( root, 'ssh/username', 'value', 'ATTRIBUTE', 'STRING')[0] == True:
			self.ssh_username = xmlValidateAndLoad(root,'ssh/username','value','ATTRIBUTE','STRING')[1]

		if xmlValidateAndLoad( root, 'ssh/hostname', 'value', 'ATTRIBUTE', 'STRING')[0] == True:
			self.ssh_hostname = xmlValidateAndLoad(root,'ssh/hostname','value','ATTRIBUTE','STRING')[1]
		
		if xmlValidateAndLoad( root, 'ssh/password', 'value', 'ATTRIBUTE', 'STRING')[0] == True:
			self.ssh_password = xmlValidateAndLoad(root,'ssh/password','value','ATTRIBUTE','STRING')[1]
		
		# General required parameters
		if xmlValidateAndLoad( root, 'output_info/num_bundles', 'value', 'ATTRIBUTE', 'INT')[0] == True:
			self.number_bundles = xmlValidateAndLoad(root,'output_info/num_bundles','value','ATTRIBUTE','INT')[1]
		
		if xmlValidateAndLoad( root, 'output_info/compression_type', 'value', 'ATTRIBUTE', 'STRING')[0] == True:
			self.compression_type = xmlValidateAndLoad( root, 'output_info/compression_type', 'value', 'ATTRIBUTE', 'STRING')[1]
		
		if xmlValidateAndLoad( root, 'output_info/camera_type', 'value', 'ATTRIBUTE', 'STRING')[0] == True:
			self.camera_type = xmlValidateAndLoad( root, 'output_info/camera_type', 'value', 'ATTRIBUTE', 'STRING')[1]
		
		if xmlValidateAndLoad( root, 'output_info/increment', 'value', 'ATTRIBUTE', 'INT')[0] == True:
			self.gs_increment = xmlValidateAndLoad( root, 'output_info/increment', 'value', 'ATTRIBUTE', 'INT')[1]
		
				
		# Path Information
		if xmlValidateAndLoad( root, 'path_info/input_base' , 'value', 'ATTRIBUTE', 'STRING')[0] == True:
			self.input_base = xmlValidateAndLoad( root, 'path_info/input_base' , 'value', 'ATTRIBUTE', 'STRING')[1]
		
		if xmlValidateAndLoad( root, 'path_info/input_path' , 'value', 'ATTRIBUTE', 'STRING')[0] == True:
			self.input_path = xmlValidateAndLoad( root, 'path_info/input_path' , 'value', 'ATTRIBUTE', 'STRING')[1]
		
		if xmlValidateAndLoad( root, 'path_info/output_base', 'value', 'ATTRIBUTE', 'STRING')[0] == True:
			self.output_base = xmlValidateAndLoad( root, 'path_info/output_base', 'value', 'ATTRIBUTE', 'STRING')[1]

		if xmlValidateAndLoad( root, 'path_info/output_path', 'value', 'ATTRIBUTE', 'STRING')[0] == True:
			self.output_path = xmlValidateAndLoad( root, 'path_info/output_path', 'value', 'ATTRIBUTE', 'STRING')[1]
		
		# GS1 Specific
		if xmlValidateAndLoad( root, 'gs1_specific/number_eo_directories', 'value', 'ATTRIBUTE', 'INT')[0] == True:
			self.num_eo_camera_directories = xmlValidateAndLoad( root, 'gs1_specific/number_eo_directories', 'value', 'ATTRIBUTE', 'INT')[1]
		
		if xmlValidateAndLoad( root, 'gs1_specific/number_ir_directories', 'value', 'ATTRIBUTE', 'INT')[0] == True:
			self.num_ir_camera_directories = xmlValidateAndLoad( root, 'gs1_specific/number_ir_directories', 'value', 'ATTRIBUTE', 'INT')[1]
		
		if xmlValidateAndLoad( root, 'gs1_specific/number_eo_images_per_camera', 'value', 'ATTRIBUTE', 'INT')[0] == True:
			self.num_eo_images_per_cam = xmlValidateAndLoad( root, 'gs1_specific/number_eo_images_per_camera', 'value', 'ATTRIBUTE', 'INT')[1]
		
		if xmlValidateAndLoad( root, 'gs1_specific/number_ir_images_per_camera', 'value', 'ATTRIBUTE', 'INT')[0] == True:
			self.num_ir_images_per_cam = xmlValidateAndLoad( root, 'gs1_specific/number_ir_images_per_camera', 'value', 'ATTRIBUTE', 'INT')[1]
		
		
		# GS2 Specific
		if xmlValidateAndLoad( root, 'gs2_specific/number_slices'  , 'value', 'ATTRIBUTE', 'INT')[0] == True:
			self.number_slices = xmlValidateAndLoad( root, 'gs2_specific/number_slices'  , 'value', 'ATTRIBUTE', 'INT')[1]
		
		if xmlValidateAndLoad( root, 'gs2_specific/number_collects', 'value', 'ATTRIBUTE', 'INT')[0] == True:
			self.number_collects = xmlValidateAndLoad( root, 'gs2_specific/number_collects', 'value', 'ATTRIBUTE', 'INT')[1]
		
		if xmlValidateAndLoad( root, 'gs2_specific/eo_fism_count'  , 'value', 'ATTRIBUTE', 'INT')[0] == True:
			self.eo_fism_count = xmlValidateAndLoad( root, 'gs2_specific/eo_fism_count'  , 'value', 'ATTRIBUTE', 'INT')[1]
		
		if xmlValidateAndLoad( root, 'gs2_specific/ir_fism_count'  , 'value', 'ATTRIBUTE', 'INT')[0] == True:
			self.ir_fism_count = xmlValidateAndLoad( root, 'gs2_specific/ir_fism_count'  , 'value', 'ATTRIBUTE', 'INT')[1]
		

		##########################################
		#       PARSE COMMAND-LINE OPTIONS       #
		##########################################
		# begin sweeping over items, looking for matching parameters
		command_args = sys.argv[1:]
		while( len(command_args) > 0 ):
			
			
			# Number of Bundles
			if command_args[0][:16] == '-number_bundles=':
				self.number_bundles = int(command_args[0][16:])
				command_args = command_args[1:]
			
			# Compression Type
			elif command_args[0][:18] == '-compression_type=':
				self.compression_type = command_args[0][18:]
				command_args = command_args[1:]
			
			# Camera Type
			elif command_args[0][:13] == '-camera_type=':
				self.camera_type = command_args[0][13:]
				command_args = command_args[1:]

			# Increment
			elif command_args[0][:11] == '-increment=':
				self.increment = int(command_args[0][11:])
				command_args = command_args[1:]
	
			# Debug Level
			elif command_args[0][:13] == '-debug_level=':
				self.debug_level = int(command_args[0][13:])
				command_args = command_args[1:]
			
			# Log State
			elif command_args[0][:11] == '-log_state=':
				self.log_state = bool(int(command_args[0][11:]))
				command_args = command_args[1:]
			
			# Log Location
			elif command_args[0][:14] == '-log_location=':
				self.log_location = command_args[0][14:]
				command_args = command_args[1:]
			
			# SSH State
			elif command_args[0][:11] == '-ssh_state=':
				self.ssh_state = int(command_args[0][11:])
				command_args = command_args[1:]
			
			# SSH Username
			elif command_args[0][:14] == '-ssh_username=':
				self.ssh_username = command_args[0][14:]
				command_args = command_args[1:]

			# SSH Hostname
			elif command_args[0][:14] == '-ssh_hostname=':
				self.ssh_hostname = command_args[0][14:]
				command_args = command_args[1:]
			
			# SSH Password
			elif command_args[0][:14] == '-ssh_password=':
				self.ssh_password = command_args[0][14:]
				command_args = command_args[1:]

			# Input Base 
			elif command_args[0][:12] == '-input_base=':
				self.input_base = command_args[0][12:]
				command_args = command_args[1:]
			
			# Input Path 
			elif command_args[0][:12] == '-input_path=':
				self.input_path = command_args[0][12:]
				command_args = command_args[1:]
			
			# Output Base 
			elif command_args[0][:13] == '-output_base=':
				self.output_base = command_args[0][13:]
				command_args = command_args[1:]
			
			# Output Path
			elif command_args[0][:13] == '-output_path=':
				self.output_path = command_args[0][13:]
				command_args = command_args[1:]
        
			# Number of EO Directories
			elif command_args[0][:23] == '-number_eo_directories=':
				self.num_eo_camera_directories = int(command_args[0][23:])
				command_args = command_args[1:]
       	
			# Number of IR Directories
			elif command_args[0][:23] == '-number_ir_directories=':
				self.num_ir_camera_directories = int(command_args[0][23:])
				command_args = command_args[1:]
        	
			# Number EO Images Per Camera
   			elif command_args[0][:29] == '-number_eo_images_per_camera=':
				self.num_eo_images_per_cam = int(command_args[0][29:])
				command_args = command_args[1:]
        
			# Number IR Images Per Camera
   			elif command_args[0][:29] == '-number_ir_images_per_camera=':
				self.num_ir_images_per_cam = int(command_args[0][29:])
				command_args = command_args[1:]
        
			# Number Slices
   			elif command_args[0][:15] == '-number_slices=':
				self.number_slices = int(command_args[0][15:])
				command_args = command_args[1:]
        
			# Number Collects
   			elif command_args[0][:17] == '-number_collects=':
				self.number_collects = int(command_args[0][17:])
				command_args = command_args[1:]
        	
			# Number of EO FISM's Per Count
			elif command_args[0][:15] == '-eo_fism_count=':
				self.eo_fism_count = int(command_args[0][15:])
				command_args = command_args[1:]
			
			# Number of IR FISM's Per Count
			elif command_args[0][:15] == '-ir_fism_count=':
				self.ir_fism_count = int(command_args[0][15:])
				command_args = command_args[1:]
			
			# Help
			elif 'help' in command_args[0]:
				self.usage();
				sys.exit(0)

			# throw an exception if an option is not recognized
			else:
				self.usage()
				raise Exception('ERROR: Unknown parameter: ' + command_args[0])
		
		
		# Make sure that we validate our results
		self.validate()
			

	################################################
	#-      Print Instructions On How To Use 	  -#
	################################################
	def usage( self ):
		"""
		Usage function for the program
		"""

		print sys.argv[0] + ' [options]'
		print ''
		print '   General Options'
		print '      -number_bundles=<value>    Number of image bundles [3+]'
		print '      -compression_type=<value>  Type of compression     [zip]'
		print '      -camera_type=<value>       Type of camera          [EO,IR]'
		print '      -increment=<value>         GS Increment Value      [1,2]'
		print ''
		print '   Debugging Information'
		print '      -debug_level=<value>       Debugging State  [0-None,1-Major,2-Minor]'
		print '      -log_state=<value>         Do Logging       [0-Do Not Log, 1-Do Logging]'
		print '      -log_location=<value>      Filename of log'
		print ''
		print '   SSH Information'
		print '      -ssh_state=<value>         To Do SSH         [0-send via ssh,1-don\'t send]'
		print '      -ssh_username=<value>      SSH Username      [Account Username]'
		print '      -ssh_hostname=<value>      SSH Host Account  [Host Account Name]'
		print '      -ssh_password=<value>      SSH Acct Password [SSH Password]'
		print ''
		print '   Path Information'
		print '      -input_base=<value>        Base Path of input directory'
		print '      -input_path=<value>        Remaining Path of input directory'
		print '      -output_base=<value>       Base Path of output directory'
		print '      -output_path=<value>       Remaining Path of output directory'
		print '   NOTE:  if SSH is enabled, the output will be used on the host machine.'
		print ''
		print '   GS1 Configuration Information'
		print '      -number_eo_directories=<value> Number of eo directories [1+]'
		print '      -number_ir_directories=<value> Number of ir directories [1+]'
   		print '      -number_eo_images_per_camera=<value> Number of images per directory [1+]'
   		print '      -number_ir_images_per_camera=<value> Number of images per directory [1+]'
		print ''
		print '   GS2 Configuration Information'
   		print '      -number_slices=<value>     Number of GPU Slices [1+]'
   		print '      -number_collects=<value>   Number of collects per slice [1+]'
		print '      -eo_fism_count=<value>     Number of EO FISHs'
		print '      -ir_fism_count=<value>     Number of IR FISMs'
		

	###############################################################
	#        Print the Configuration and all Options to file      #
	###############################################################
	def __str__( self ):
		"""
		Print function for the Command Line option class
		"""

		strout  = 'Current Configuration Values ' + '\n'
		strout += '\n'
		strout += '   Collection Parameters \n'
		strout += '   - Number of bundles     : ' + str(self.number_bundles) + '\n'
		strout += '   - Compression Type      : ' + self.compression_type    + '\n'
		strout += '   - Camera Type           : ' + self.camera_type         + '\n'
		strout += '   - Increment             : ' + str(self.gs_increment)      + '\n'
		strout += '\n'
		strout += '   SSH Paramters \n'
		strout += '   - SSH Status            : ' + str(self.ssh_state) + '\n'
		strout += '   - SSH Username          : ' + self.ssh_username + '\n'
		strout += '   - SSH Hostname          : ' + self.ssh_hostname + '\n'
		strout += '   - SSH Password          : ' + self.ssh_password + '\n'
		strout += '\n'
		strout += '   Path Parameters \n'
		strout += '   - Input Base            : ' + self.input_base   + '\n'
		strout += '   - Input Path            : ' + self.input_path   + '\n'
		strout += '   - Output Base           : ' + self.output_base  + '\n'
		strout += '   - Output Path           : ' + self.output_path  + '\n'
		strout += '\n'
		strout += '   GS1 Parameters \n'
		strout += '   - number_eo_directories : ' + str(self.num_eo_camera_directories) + '\n'
		strout += '   - number_ir_directories : ' + str(self.num_ir_camera_directories) + '\n'
		strout += '   - number_eo_images_per_camera : ' + str(self.num_eo_images_per_cam) + '\n'
		strout += '   - number_ir_images_per_camera : ' + str(self.num_ir_images_per_cam) + '\n'
		strout += '\n'
		strout += '   GS2 Parameters \n'
		strout += '   - number_slices         : ' + str(self.number_slices)   + '\n'
		strout += '   - number_collects       : ' + str(self.number_collects) + '\n'
		strout += '   - eo_fism_count         : ' + str(self.eo_fism_count) + '\n'
		strout += '   - ir_fism_count         : ' + str(self.ir_fism_count) + '\n'
		strout += '\n'
		strout += '   Debugging Information \n' 
		strout += '   - Debug Level           : ' + str(self.debug_level) + '\n'
		strout += '   - Log State             : ' + str(self.log_state)   + '\n'
		strout += '   - Log Location          : ' + self.log_location     + '\n'
		return strout


	###############################################################
	#              Ensure the config parameters are okay          #
	###############################################################
	def validate( self ):
		
		if self.debug_level < 0 or self.debug_level > 4:
			raise Exception('Debug Level has invalid setting')
		
		if self.number_bundles < 0:
			raise Exception('Number Bundles has invalid setting')

		if self.compression_type != 'zip':
			raise Exception('Compression Type has invalid setting')

		if self.camera_type != 'EO' and self.camera_type != 'IR':
			raise Exception('Camera Type has invalid setting')
		
		if self.gs_increment != 1 and self.gs_increment != 2:
			raise Exception('Increment has invalid setting')
	
		if self.num_eo_camera_directories < 1:
			raise Exception('Number EO Cam Directories has invalid setting')
		
		if self.num_eo_camera_directories < 1:
			raise Exception('Number IR Cam Directories has invalid setting')
		
		if self.num_eo_images_per_cam < 1:
			raise Exception('Number EO Images Per Cam Directory has invalid setting')

		if self.num_ir_images_per_cam < 1:
			raise Exception('Number EO Images Per Cam Directory has invalid setting')
		
		if self.number_slices < 1:
			raise Exception('Number of GPU Slices')
			
		if self.number_collects < 1:
			raise Exception('Number of GPU Slice Collections')

		if self.eo_fism_count < 1:
			raise Exception('Number of EO FISMs is incorrect')

		if self.ir_fism_count < 1:
			raise Exception('Number of EO FISMs is incorrect')
		
		# Condition the paths
		# if the path starts with a /, then cancel out the base
		if self.input_path[0] == '/':
			self.input_path = self.input_path[1:]
			self.input_base = '/'
		
		if self.output_path[0] == '/':
			self.output_path = self.output_path[1:]
			self.output_base = '/'
		
		# remove all trailing / 
		if self.input_base[-1] == '/':  self.input_base  = self.input_base[:-1]
		if self.input_path[-1] == '/':  self.input_path  = self.input_path[:-1]
		if self.output_base[-1] == '/': self.output_base = self.output_base[:-1]
		if self.output_path[-1] == '/': self.output_path = self.output_path[:-1]





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
def validityCheckPreList( directory ):
	"""
	This function does a basic check to make sure that the directory we have passed into the program
	actually exists and furthermore that the directory is actually a directory. 
	"""

	# Make sure that the directory we are searching for exists
	if os.path.exists( directory ) == False:
		
		log.write( log.MAJOR, directory + ' does not exist')
		raise Exception(log.create_log_message( log.MAJOR, directory + ' does not exist'))

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
def find_camera_directory( directory, options ):
	

	# extract the contents of the directory
	contents = os.listdir(directory);

	# remove all non-directories
	directories = [elem for elem in contents if os.path.isdir(directory+'/'+elem) == True]
		
	print 'dirs: ' + str(directories)
	sys.exit(0)

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
	options = ConfigOptions( sys.argv[1:] )
	
	# Configure the Logger
	global log
	log = Logger( options.debug_level, options.log_state, options.log_location )
	
	try:
		# Print the configuration options to file
		log.write(log.INFO, str(options))
		
		# make sure the data is in an intelligible format
		validityCheckPreList( options.input_base + '/' + options.input_path);
		
		# build a list of camera directories
		camera_directories = find_camera_directory( options.input_base + '/' + options.input_path, options )

	except Exception as EX:
		pass

	finally:
		# Close the log file
		log.close()

	return


	
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
	if options.num_bundles > len(image_tuples):
		if options.debug_level >= 1:
			print 'WARNING: image data has fewer image sets than requested.  Returning ' + str(len(image_tuples)) + ' of ' + str(options.num_bundles)
			print ''

		options.output_code = len(image_tuples)
	
	options.num_bundles = min( options.num_bundles, len(image_tuples))


	# now that we have a list of images, we need to start dividing image sets
	bundle_step = len(image_tuples) / options.num_bundles	

	# Here we need to consider if we are going to transfer the file via scp or if we will
	# write it to local disk
	if options.ssh == True:
		zip_filename = './bundle' + options.compression_type
	else:
		zip_filename = options.output_base + options.output_path + options.compression_type

	# create output data
	zf = zipfile.ZipFile( zip_filename , 'w')
	
	if options.debug_level >= 1:
		print 'writing files to : ' + zip_filename

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

	# If we need to send the file via scp, then we need to create the objects for passing it
	if options.ssh == True:

		# create the SSH object
		ssh = paramiko.SSHClient()
		ssh.set_missing_host_key_policy( paramiko.AutoAddPolicy() )
		ssh.connect( options.ssh_host, username=options.ssh_user, password=options.ssh_password)
		
		# create path for destination
		ssh_path = options.output_base + options.output_path + options.compression_type
		
		if options.debug_level >= 1:
			print 'destination for sftp: ' + ssh_path

		# create sftp process
		sftp = ssh.open_sftp()
		sftp.put( './bundle'+options.compression_type, ssh_path)
		sftp.close()
		ssh.close()

		os.remove('./bundle'+options.compression_type)

	else:
		pass
	
	sys.exit(options.output_code)

if __name__ == "__main__":
	main()

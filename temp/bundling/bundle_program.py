#! /usr/bin/env python

import sys, os, time, tarfile, zipfile, paramiko, xml.etree.ElementTree as xml
from collections import deque
import glob

months  = ['JAN','FEB','MAR','APR','MAY','JUN','JUL','AUG','SEP','OCT','NOV','DEC']
_cameras = ['cam1','cam2','cam3','cam4','cam5']

NEWLINE = '\n'
log = []

MAX_BUNDLES=100
BUNDLE_OVERKILL=5#Number of times past to build for
MAX_TIME=2 # HOURS


cameras = None

holdForPrint = False

#################################
#         Color Console         #
#################################
class COLOR:
	
	GREEN  = '\033[92m'
	RED    = '\033[91m'
	YELLOW = '\033[93m'
	BLUE   = '\033[94m'
	ENDC   = '\033[0m'

color = COLOR()

def print_help_option( gap, tag, value, comment ):
	
	print gap + COLOR.GREEN + tag + COLOR.RED + value + COLOR.BLUE + comment + COLOR.ENDC


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
#            Logging Exception            #
###########################################
class LogException( Exception ):
	level = 0
	message = ''

	def __init__(self, level, message):
		
		self.level   = level;
		self.message = message
		log.write(level, message)
	
	def __str__(self):
		return log.create_log_message(self.level, self.message)


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
	
	collection_name = ''
	gs_directories = []

	

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
		if xmlValidateAndLoad( root, 'output_info/number_bundles', 'value', 'ATTRIBUTE', 'INT')[0] == True:
			self.number_bundles = xmlValidateAndLoad(root,'output_info/number_bundles','value','ATTRIBUTE','INT')[1]
		
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

		if xmlValidateAndLoad( root, 'path_info/output_filename', 'value', 'ATTRIBUTE', 'STRING')[0] == True:
			self.output_path = xmlValidateAndLoad( root, 'path_info/output_filename', 'value', 'ATTRIBUTE', 'STRING')[1]
		
		if xmlValidateAndLoad( root, 'path_info/collect_name', 'value', 'ATTRIBUTE', 'STRING')[0] == True:
			self.collect_name = xmlValidateAndLoad( root, 'path_info/collect_name', 'value', 'ATTRIBUTE', 'STRING')[1]
		
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
		
		
		# Pull Appropriate Directory Structure
		structure = "gs" + str(self.gs_increment) + "_structure"
		basenode = root.find(structure + '/directory')
		
		# Here we iterate until we grab a camera directory pattern
		while basenode != None:
			
			# Grab the type of directory
			dtype = basenode.attrib.get('type')
			print dtype

			# raise an exception if the type is none
			if dtype == None:
				raise Exception('Camera Directory was not found. Invalid file structure')

			# Check if the item is a static directory
			elif dtype == 'static':
				#if so, then append it to the directory list 
				self.gs_directories.append( ('STATIC', basenode.attrib.get('pattern')))

			# Check if the item is a variable directory
			elif dtype == 'variable':
				#if so, then append it to the directory list
				self.gs_directories.append( ('VARIABLE', basenode.attrib.get('pattern')))

			elif dtype == 'camera':
				#if so, then append it to the directory list
				self.gs_directories.append( ('CAMERA', basenode.attrib.get('pattern')))
				# end the search 
				break;

			else:
				raise Exception("Unknown parameter")
			
			# Grab the child
			basenode = basenode.find('directory')

	

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
				print 'PASSWORD: ' + command_args[0]
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

			# Gorgon Stare Collection Number
			elif command_args[0][:14] == "-collect_name=":
				self.collection_name = command_args[0][14:]
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
		
		gap = '      '
		print sys.argv[0] + ' [options]'
		print ''
		print '   '+ COLOR.BLUE + 'General Options' + COLOR.ENDC 
		print_help_option( gap, '-number_bundles='  , '<integer>', '   Number of image bundles [3+]')
		print_help_option( gap, '-compression_type=', '<string>' , '  Type of compression     [zip]')
		print_help_option( gap, '-camera_type='     , '<string>' , '       Type of camera          [EO,IR]')
		print_help_option( gap, '-increment='       , '<integer>', '        GS Increment Value      [1,2]')
		print ''
		print '   ' + COLOR.BLUE + 'Debugging Information' + COLOR.ENDC
		print_help_option( gap, '-debug_level='     , '<integer>', '      Debugging State  [0-None,1-Major,2-Minor]')
		print_help_option( gap, '-log_state='       , '<integer>', '        Do Logging       [0-Do Not Log, 1-Do Logging]')
		print_help_option( gap, '-log_location='    , '<string>' , '      Filename of log')
		print ''
		print '   ' + COLOR.BLUE + 'SSH Information' + COLOR.ENDC
		print_help_option( gap, '-ssh_state='       ,'<integer>' , '        To Do SSH         [0-send via ssh,1-don\'t send]')
		print_help_option( gap, '-ssh_username='    ,'<string>'  , '      SSH Username      [Account Username]')
		print_help_option( gap, '-ssh_hostname='    ,'<string>'  , '      SSH Host Account  [Host Account Name]')
		print_help_option( gap, '-ssh_password='    ,'<string>'  , '      SSH Acct Password [SSH Password]')
		print ''
		print '   ' + COLOR.BLUE + 'Path Information' + COLOR.ENDC
		print_help_option( gap, '-input_base='      ,'<string>'  , '        Base Path of input directory')
		print_help_option( gap, '-input_path='      ,'<string>'  , '        Remaining Path of input directory')
		print_help_option( gap, '-output_base='     ,'<string>'  , '       Base Path of output directory')
		print_help_option( gap, '-output_path='     ,'<string>'  , '       Remaining Path of output directory')
		print COLOR.BLUE + '   NOTE:  if SSH is enabled, the output will be used on the host machine.' + COLOR.ENDC
		print ''
		print COLOR.BLUE + '   GS1 Configuration Information' + COLOR.ENDC
		print_help_option( gap, '-number_eo_directories=','<integer>', ' Number of eo directories [1+]')
		print_help_option( gap, '-number_ir_directories=','<integer>', ' Number of ir directories [1+]')
   		print_help_option( gap, '-number_eo_images_per_camera=','<integer>',' Number of images per directory [1+]')
   		print_help_option( gap, '-number_ir_images_per_camera=','<integer>',' Number of images per directory [1+]')
		print ''
		print COLOR.BLUE + '   GS2 Configuration Information' + COLOR.ENDC
   		print_help_option( gap, '-number_slices='  ,'<integer>','     Number of GPU Slices [1+]')
   		print_help_option( gap, '-number_collects=','<integer>','   Number of collects per slice [1+]')
		print_help_option( gap, '-eo_fism_count='  ,'<integer>','     Number of EO FISHs')
		print_help_option( gap, '-ir_fism_count='  ,'<integer>','     Number of IR FISMs')
		

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
	raw_data         = []
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
	
		self.raw_data = imgname
		instr = os.path.basename(imgname)
		self.input_string = instr

		# set the date DDMMMYY [0-6]
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

		# get program_code [7-8]
		self.program_code = instr[0:2]
		instr = instr[2:]

		# get sortie_number [9-10]
		self.sortie_number = instr[0:2]
		instr = instr[2:]

		# get scene number [11-16]
		self.scene_number = int(instr[:5])
		instr = instr[5:]

		# get DoD producer code [16-17]
		self.producer_code = instr[:2]
		instr = instr[2:]

		# get product number [15-21]
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

def compareTACID( tacidA, tacidB ):
	"""
	Compares TACIDs by their scene values.
	-1 - A < B
	0  - A = B
	1  - A > B
	"""
	
	if   tacidA.producer_sn < tacidB.producer_sn: # the pn is smaller in A, don't worry about scene number
		return -1
	
	elif tacidA.producer_sn == tacidB.producer_sn: # the pn is smaller in A, don't worry about scene number
		if    tacidA.scene_number < tacidB.scene_number: # the sn is smaller in A
			return -1
		elif  tacidA.scene_number == tacidB.scene_number: # they are equal
			return 0
		else: 
			return 1
			
	else:
		return 1


def _listdir(cdir):
	
	return os.listdir(cdir)
	return [ os.path.split(output)[1] for output in glob.glob(cdir + '/*') ]

###########################################################################################################
#
###########################################################################################################
class Camera:
	base = ''
	dir_tree = deque()

	def __init__(self, dir_name ):
		""" 
		Construct the directory tree for this object
		"""

		# Set the base directory name
		self.base = dir_name

		# Pull the contents of the directory
		contents = _listdir(dir_name)
		contents.sort()
		contents.reverse()

		self.dir_tree = deque()
		for c in contents:
			self.dir_tree.append((dir_name + '/' + c, 0))
	
	def pop_item(self):
		img = self.dir_tree.popleft()
		return img
	
	def peek_item(self):
		return self.dir_tree[0][0]


	def dump(self):
		"""
		This function will delete everything in the dir_tree
		"""
		dir_tree.clear()


	def step(self):
		"""
		The camera will enter the directory
		"""
		
		# Pop off the current directory item
		data = self.dir_tree.popleft()
		cdir = data[0]
		depth= data[1]
		
		# check if current position is a file or directory
		isDir = os.path.isdir(cdir)
		
		# if its a directory, lets step into it
		if isDir == True:

			# Grab the contents of the directory and sort
			contents = _listdir(cdir)  #[elem for elem in items if os.path.isdir(cdir+'/'+elem) == True]
			contents.sort()
			contents.reverse()

			# Insert onto stack
			for c in contents:
				self.dir_tree.appendleft((cdir + '/' + c, depth+1))

			

	def __str__(self):
		out = ''
		out += "Camera Base: " + self.base + '\n'

		return out

class Bundle:
	images = []

#------------------------------------------------#
#-    			Basic Error Checking  			-#
#------------------------------------------------#
def validityCheckPreList( directory ):
	"""
	This function does a basic check to make sure that the directory we have passed into the program
	actually exists and furthermore that the directory is actually a directory. 
	"""
	log.write(log.INFO, 'validityCheckPreList starting')

	# Make sure that the directory we are searching for exists
	if os.path.exists( directory ) == False:
		
		raise LogException( log.MAJOR, directory + ' does not exist')
	
	else:
		log.write( log.INFO, directory + ' exists')

	# make sure that it is a directory
	if os.path.isdir(directory) == False:
		print 'Error: ' + directory + ' is not a directory '
		sys.exit(-1)
	else:
		log.write( log.INFO, directory + ' is a directory')
	
	log.write(log.INFO, 'validityCheckPreList ended successfully')
	return True




def validityCheckPost( camera_roots, options ):
	"""
	This function checks that the root camera directories we have located conform
	to the requirements of the specific camera type. The primary assumption made 
	here is that the camera type has been set.
	"""
	
	# Print the log messages
	log.write( log.INFO, 'Starting validityCheckPost')

	
	# ensure that we have the proper number of directories
	if options.gs_increment == 1:
		if options.camera_type == 'EO':
			pass
		if options.camera_type == 'IR':
			pass

	elif options.gs_increment == 2:
		if options.camera_type == 'EO':
			pass
		if options.camera_type == 'IR':
			pass

	else: 
		raise LogException( log.MAJOR, 'Invalid GS Increment \'' + str(options.gs_increment) + '\' detected');

	
	log.write( log.INFO, 'Exiting validityCheckPost successfully')



#--------------------------------------------------------#
#-   Check Directory Pattern Match                      -#
#--------------------------------------------------------#
def dirPatternMatch( d1, match_directory, collect_name ):
	
	# extract the match directory name
	d2 = match_directory[1]

	# Check the lengths
	if len(d1) != len(d2):
		return False;
	
	# if we have a static directory, then just compare the names
	if match_directory[0] == 'STATIC':
		for x in xrange(0, len(d1)):

			if d1[x] == "#":
				if int(d2[x]) >= 0 or int(d2[x]) < 10:
					continue
			elif d2[x] == "#":
				if int(d1[x]) >= 0 or int(d1[x]) < 10:
					continue
			elif d1[x] != d2[x]:
				return False
	
	elif match_directory[0] == 'VARIABLE':
		# Make sure that the pattern in d2 matches the length of d1
		# Since we have a variable directory, we need to make sure the 
		# input directory (d1) matches the collect_name
		for x in xrange( 0, len(d1)):

			if d1[x] == '#':
				if int(collect_name[x]) >= 0 or int(collect_name[x]) < 10:
					continue
			elif collect_name[x] == '#':
				if int(d1[x]) >= 0 or int(d1[x]) < 10:
					continue
			elif d1[x] != collect_name[x]:
				return False
		
	elif match_directory[0] == 'CAMERA':
		# if we are here, we have obvious overflow.  This means we should stop
		# this path immediately
		return False

	else:
		raise Exception('Line 900, UNKNOWN OPTION')

	return True


#------------------------------------------------------#
#-    Find Camera Directory		     				  -#
#-									     		  	  -#
#-    Look for the baseline camera directory          -#
#-    - This is achieved by looking at the structure  -#
#       as defined in the xml file and comparing it.  -#
#------------------------------------------------------#
def find_camera_directory( directory, options, camera_list ):
	
	# extract the contents of the directory
	contents = os.listdir(directory);
	
	# Build an empty directory tree object
	dir_stack = []
	
	# remove all non-directories
	directories = [elem for elem in contents if os.path.isdir(directory+'/'+elem) == True]
	directories.sort()

	# Iterate through every directory we queried
	for d in directories:
		
		# reset the found flag
		isCamDirectory = False
		
		if options.gs_increment == 1:
			# Test for Inc 1 Imagery
			# 
			# a camera directory for inc 1 must be a camera directory with a single digit
			try:
				if len(d) == 4 and d[:3] == 'cam' and int(d[3]) > 0 and int(d[3]) < max(options.num_eo_camera_directories, options.num_ir_camera_directories):
					
					dir_stack.append( Camera(directory + '/' + d))
					isCamDirectory = True
			except ValueError:
				pass
		
		
		elif options.gs_increment == 2:
			# Test for Inc 2 Imagery
			#
			# a camera directory for Inc 2 must be a camera directory with 3 digits
			try:
				if options.camera_type == 'EO':
					if len(d) == 6 and d[:3] == 'cam' and int(d[3:]) >= 0 and int(d[3:]) < options.eo_fism_count:
						
						dir_stack.append( Camera(directory + '/' + d))
						isCamDirectory = True
				
				elif options.camera_type == 'IR':
					if len(d) == 6 and d[:3] == 'cam' and int(d[3:]) >= 0 and int(d[3:]) < options.ir_fism_count:
						
						dir_stack.append( Camera(directory + '/' + d))
						isCamDirectory = True
				else:
					raise LoggerException( log.MAJOR, 'Unknown camera type ' + options.camera_type + ' expected')

			except ValueError:
				pass

		# if the camera directory is not a camera directory, then step into it
		if isCamDirectory == False and len(camera_list) > 0:
			
			# Make sure the pattern matches the directory structure we defined in the xml file
			if dirPatternMatch( d, camera_list[0], options.collect_name ) == True:
				dir_stack.extend(find_camera_directory( directory + '/' + d, options, camera_list[1:] ))
			
	# end of the directory iteration

	return dir_stack





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


######################################
#-      Prune The Camera List       -#
######################################
def prune_camera_list( cameras, options ):
	""" 
	compare the contents of each camera
	"""

	# iterate the list, looking for matching images
	breakNow = False
	output = []
	
	while True:
		
		# start with the smallest value
		minCamVal = os.path.split(cameras[0].dir_tree[0][0])[1]
		minCamDep = cameras[0].dir_tree[0][1]
		minCamIdx = 0
		minType   = os.path.isdir(cameras[0].dir_tree[0][0])
		minID = None

		# if the minType is a file, then we need to compute the scene number and product number
		if minType == False: 
			minID = TACID(cameras[0].dir_tree[0][0])
		

		# This flag will tell us if the current directories are valid, if 
		# any directory is different, the flag will be set
		bad_value_found = False
		
		for x in xrange( 0, len(cameras)):
			
			#----------------------------------------------------------------------#
			#-  Note: If the camera has no more items in its tree, then it means  -#
			#-  that we can dump the remaining camera directory contents for the  -#
			#-  other items.                                                      -#
			#----------------------------------------------------------------------#
			if len(cameras[x].dir_tree) <= 0:
				
				#----------------------------------------------------------#
				#-   Since we have no images, we need to dump everything  -#
				#----------------------------------------------------------#
				for y in xrange( 0, len(cameras)):
					cameras[x].dump()
				return True
	
			# gather some test information
			testDep    = cameras[x].dir_tree[0][1]
			testCamVal = os.path.split(cameras[x].dir_tree[0][0])[1]
			testCamDep = cameras[0].dir_tree[0][1]
			testCamIdx = x
			testType   = os.path.isdir( cameras[x].dir_tree[0][0])
			testID = None
			
			global holdForPrint
			holdForPrint = False
			#if minCamVal == '00' and x < 10:
			#	holdForPrint = True
		
			if holdForPrint == True:
				print 'comparing ', minCamVal, '  with  ', testCamVal
				print cameras[x].dir_tree[0]
				print 'current step: ', x
				print 'testCamVal  : ', testCamVal
				print 'minCamVal   : ', minCamVal
				print 'bad value   : ', bad_value_found
				raw_input('hold')

			
			# if we are comparing against a file, we need to have its TACID
			if testType == False:
				testID = TACID(cameras[x].dir_tree[0][0])
	
			
			#-------------------------------------------------------#
			#-           Here I outline the decision tree          -#
			#-------------------------------------------------------#
			

			#--------------------------------------------------------------------#
			#-   Do both the test item and minimum item have the same depth?    -#
			#--------------------------------------------------------------------#
			if   minCamDep == testCamDep:
					
				if holdForPrint == True:
					print 'equal depth'

				#---------------------------------------------------------------------#
				#-   Assuming they have the same depth, are they both directories?   -#
				#---------------------------------------------------------------------#
				if testType == True and minType == True:
					
					if holdForPrint == True:
						print 'both directories'

				    #-----------------------------------------------------------------#
					#-  Since they are both directories with the same depth, do      -#
					#-  they have the same name?                                     -#
					#-----------------------------------------------------------------#
					if minCamVal == testCamVal:
						
						if holdForPrint == True:
							print 'both same dir name'

						# both are equal, continue
						continue


					elif minCamVal < testCamVal:
						
						if holdForPrint == True:
							print 'min cam is less'

						# minCam is still the min cam, but the test value means there is an error
						# We will need to delete the minCamVal later
						bad_value_found = True
						continue
					
					else:
						
						if holdForPrint == True:
							print 'testCamVal is less'

						# the test camera is now the min camera, lets replace them
						minCamVal = testCamVal
						minCamDep = testCamDep
						minCamIdx = testCamIdx
						minType   = testType
						minID     = testID
						bad_value_found = True
						continue

				#-------------------------------------------------------------#
				#-  if they are not both directories, are they both files?   -#
				#-------------------------------------------------------------#
				elif testType == False and minType == False:
					
					#-----------------------------------------------------------------------------#
					#- Since they are both files, do they share the same scene and producer_sn?  -#
					#-----------------------------------------------------------------------------#
					compResult = compareTACID( testID, minID );
					
					if compResult == 0:
						
						# both are equal, continue
						continue
					
					elif compResult < 0:
						
						# The testID is smaller than the minimum. We need to make the testID the new minimum camera
						minCamVal = testCamVal
						minCamDep = testCamDep
						minCamIdx = testCamIdx
						minType   = testType
						minID     = testID
						bad_value_found = True
						continue
					
					else:
						
						# The minimum camera is still the minimum camera.  Lets ensure we set the bad val flag
						# and move on.  We can leave the min camera in place
						bad_value_found = True
						continue

				
				#-------------------------------------------------------------------------------------------#
				#-   If they are different types, we need to automatically prune the directory, as we are  -#
				#-   assuming that directories will always be sorted before files.                         -#
				#-------------------------------------------------------------------------------------------#
				elif testType == True:  # assuming the minType is a file
					
					# The testID is smaller than the minimum. We need to make the testID the new minimum camera
					minCamVal = testCamVal
					minCamDep = testCamDep
					minCamIdx = testCamIdx
					minType   = testType
					minID     = testID
					bad_value_found = True
					continue

				
				else:  # Assuming the minType is a directory and minType is a file
					bad_value_found = True
					continue

		# end of for x in xrange( 0, len(cameras))

		
		#-------------------------------------------------------------------------------#
		#-   If the bad_value_found flag is still false, then we are safe to continue  -#
		#-------------------------------------------------------------------------------#
		if bad_value_found == False:
			return False

		#-----------------------------------------------------------------------------------#
		#-   If the bad_value_found_flag was found, then we need to delete the minCamIdx   -#
		#-   camera's top item.                                                            -#
		#-----------------------------------------------------------------------------------#
		cameras[minCamIdx].pop_item()
		


		#   end of while loop

	raw_input('end of prune loop')

	return False



##########################################################################
#                      Zip Compression File Builder                      #
##########################################################################
def write_zipfile( output_filename, image_tuples, bundle_step, options ):

	# open zipfile object
	log.write( log.INFO, 'Opening zip file : ' + output_filename)
	zf = zipfile.ZipFile( output_filename, 'w')
	
	# create current position pointer
	current_pos = 0
	for x in range(0, options.number_bundles):
		
		# increment position
		current_pos = bundle_step * x
		for y in range( 0, len(image_tuples[current_pos])):
			
			# add file to tarball
			zf.write( image_tuples[current_pos][y].raw_data, 'bundles/bundle'+str(x)+'/'+image_tuples[current_pos][y].input_string)

	# close the zip file
	zf.close()


##################################################
#-					Main Driver					-#
##################################################
def main():

	# Parse command-line options
	options = ConfigOptions( sys.argv[1:] )
	camera_list = options.gs_directories

	# Configure the Logger
	global log
	log = Logger( options.debug_level, options.log_state, options.log_location )
	
	image_bundles = deque()

	for x in xrange(0,1):
	#try:
		# Print the configuration options to file
		log.write(log.INFO, str(options))
		
		# make sure the data is in an intelligible format
		validityCheckPreList( options.input_base + '/' + options.input_path);
		
		# build a list of camera directories
		log.write( log.INFO, 'running find_camera_directory on directory: ' + options.input_base + '/' + options.input_path)
		global cameras
		cameras = find_camera_directory( options.input_base + '/' + options.input_path, options, camera_list )
		log.write( log.INFO, 'find_camera_directory exited successfully')
		
		# iterate each camera directory concurrently to find matching image bundles
		while True:
			
			# have each cam object step through the next item
			for x in xrange(0, len(cameras)):
				cameras[x].step()
			
			print 'top of list'
			print cameras[0].dir_tree
			print cameras[0].dir_tree[0]

			# Compare every camera and remove until we have a valid triple
			camerasEmpty = prune_camera_list( cameras, options )
		
			# if the cameras are empty, then we are done looping
			if camerasEmpty == True:
				break

			# if the top most item is not an image, then skip
			if os.path.isfile(cameras[0].peek_item()) == False:
				continue
			
			print 'adding'
			print len(cameras[x].dir_tree)
			print cameras[x].dir_tree[0][0]

			# if the cameras still have images, it means the top of the stack is a bundle
			# pop off the bundles
			img_bundle = deque()
			for x in xrange(0, len(cameras)):
				img_bundle.append(cameras[x].peek_item())

			# add the image bundle to the image bundle list
			image_bundles.append(img_bundle)
			
			print 'total: ', len(image_bundles)
			print ''
			#raw_input('pause')

		raw_input('end of bundle searching')

		
		
		# now that we have the image sets, lets create the image bundle
		#    NOTE: A potential problem exists where we may not have enough image sets to satisfy the 
		#          request given in number of bundles variable.  This means that we must bundle what we 
		#          and send an error back notifying that we only sent 'N' values
		if options.number_bundles > len(image_tuples):
			
			log.write( log.WARNING, 'The number of valid image sets is less than request value. Returning ' + str(len(image_tuples)) + ' bundles')
			
			# set a new bundle size 
			options.number_bundles = len(image_tuples)

			# set the output code to notify the user
			options.output_code = len(image_tuples)
		

		# start dividing the image sets
		bundle_step = len(image_tuples) / options.number_bundles

		# consider if we are using ssh and create output file
		output_filename = '.'
		if options.ssh_state == True:
			output_filename = './bundle.'+options.compression_type
		else:
			output_filename = options.output_base + '/' + options.output_path + '.' + options.compression_type
	
		
		# build compressed file
		if options.compression_type == 'zip':
			
			# creating zip object
			log.write( log.INFO, 'opening zip compression object: ' + output_filename)
			write_zipfile( output_filename, image_tuples, bundle_step, options )

		elif options.compression_type == 'tar':
			raise LogException(log.MAJOR, 'TAR compression currently not supported')
		else:
			raise LogException(log.MAJOR, 'Unknown compression type')

		
		# finally, lets push this file out through ssh if selected
		if options.ssh_state == True:
				
			# create an ssh object
			ssh = paramiko.SSHClient()
			ssh.set_missing_host_key_policy( paramiko.AutoAddPolicy())
			ssh.connect( options.ssh_hostname, username=options.ssh_username, password=options.ssh_password)

			# create path for distination
			ssh_path = options.output_base + '/' + options.output_path + '.' + options.compression_type
			
			# create sftp process
			log.write( log.INFO, 'sending imagery to ' + ssh_path)
			sftp = ssh.open_sftp()
			sftp.put( './bundle'+'.'+options.compression_type, ssh_path)
			sftp.close()
			ssh.close()
			
			# clean up after ourselves
			os.remove('./bundle'+'.'+options.compression_type)


	
	#except Exception as EX:
	#	print EX
	#	options.output_code = -1
	#	return options.output_code

	#finally:
		# Close the log file
	#	log.close()

	# exit the program
	return options.output_code


if __name__ == "__main__":
	exit_code = main()
	sys.exit(exit_code)

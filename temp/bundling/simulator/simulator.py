#! /usr/bin/env python
import sys, os, errno, xml.etree.ElementTree as xml, datetime, time

NEWLINE = '\n'

##################################
#      Configuration Options     #
##################################
class Options:
	global_counter = []

	global_hz = 1
	global_program_code = 'AA'
	global_sortie_number = '00'
	global_producer_code = 'AA'
	global_camera_type   = 'XXX'
	global_project_code  = 'AA'
	global_replay_code   = 'ZZZ'
	global_extension     = '.fff'

	global_start_day   = 0
	global_start_month = 0
	global_start_year  = 0
	global_start_hour  = 0
	global_start_min   = 0

	global_debug_state   = 0
	global_log_state     = False
	global_log_location  = ".simulator.log"

	months  = ['', 'JAN','FEB','MAR','APR','MAY','JUN','JUL','AUG','SEP','OCT','NOV','DEC']
	
	# Important XML Nodes
	root = []
	layout = []

	def __init__( self, args ):

		# Parse Command-Line Arguments
		config_filename = "simulator.xml"

		# iterate through each argument
		for item in args:
			if item[:6] == '-file=':
				config_filename = item[6:]
			else:
				raise Exception("ERROR: unsupported option")


		# set configuration filename
		self.config_filename = config_filename

		# open root node
		self.root = xml.parse(config_filename)

		# Grab the important configuration options
		self.global_hz            = int(self.root.find('camera_options/frame_rate').attrib.get('value'))
		self.global_program_code  =     self.root.find('tacid/program_code').attrib.get('value')
		self.global_sortie_number =     self.root.find('tacid/sortie_number').attrib.get('value')
		self.global_producer_code =     self.root.find('tacid/producer_code').attrib.get('value')
		self.global_camera_type   =     self.root.find('tacid/product_number').attrib.get('value')
		self.global_project_code  =     self.root.find('tacid/project_code').attrib.get('value')
		self.global_replay_code   =     self.root.find('tacid/replay_code').attrib.get('value')
		self.global_extension     =     self.root.find('tacid/extension').attrib.get('value')
	
		self.global_start_day     = int(self.root.find('tacid/start_scene').attrib.get('day'))
		self.global_start_month   = int(self.root.find('tacid/start_scene').attrib.get('month'))
		self.global_start_year    = int(self.root.find('tacid/start_scene').attrib.get('year'))
		self.global_start_hour    = int(self.root.find('tacid/start_scene').attrib.get('hour'))
		self.global_start_min     = int(self.root.find('tacid/start_scene').attrib.get('minute'))
	
		self.global_debug_state   = int( self.root.find('run_options/debug_level').attrib.get('value'))
		self.global_log_state     = bool(self.root.find('run_options/log_file').attrib.get('state'))
		self.global_log_location  =      self.root.find('run_options/log_file').attrib.get('location')

		self.layout = self.root.find('layout')
	

#####################################
#                LOG    			#
#####################################
class Logger:
	
	# LEVELS
	MAJOR   = 0
	MINOR   = 1
	WARNING = 2
	INFO    = 3
	HEADING = 4

	# Internal parameters
	filename     = ''
	log_contents = ''
	logfile      = []
	state        = False
	debug_leve   = 0


	def __init__( self, options ):
		
		# check if the log file is enabled
		self.state = options.global_log_state

		#set filename 
		self.filename = options.global_log_location
		
		# if the log is enabled, open the file
		if self.state == True:
			self.open()
		
		# set the debugging level
		self.debug_level = options.global_debug_state

		# write the debugging level
		self.write( self.HEADING, 'Printing Configuration File Information'      )
		self.write( self.INFO, 'DEBUGGING LEVEL: ' + str(self.debug_level)       )
		self.write( self.INFO, 'LOGGING STATE  : ' + str(self.state)             )
		self.write( self.INFO, 'LOGGING FILE   : ' + self.filename               )
		self.write( self.INFO, 'Frame Rate     : ' + str(options.global_hz)      )
		self.write( self.INFO, 'Program Code   : ' + options.global_program_code )
		self.write( self.INFO, 'Sortie Number  : ' + options.global_sortie_number)
		self.write( self.INFO, 'Producer Code  : ' + options.global_producer_code)
		self.write( self.INFO, 'Camera Type    : ' + options.global_camera_type  )
		self.write( self.INFO, 'Project Code   : ' + options.global_project_code )
		self.write( self.INFO, 'Replay Code    : ' + options.global_replay_code  )
		self.write( self.INFO, 'File Extension : ' + options.global_extension    )


	
	def open( self ):
		
		if self.logfile == []:
			self.logfile = open( self.filename, 'w' )

	def close( self ):
		
		# open log file
		self.logfile.close()
	

	def write( self, level, message ):
		
		# create message
		output = self.create_log_message( level, message)

		# major events
		if ( level == 0 and self.debug_level >= 1 ) or ( level > 0 and self.debug_level >= 2 ):
            
			# write to console
			print( output[:-(len(NEWLINE))] )

		# write error to log if debug level and log file are allowed
		if self.state == True:
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


#----------------------------------------------#
#				Create Directory               #
#----------------------------------------------#
def create_directory( directory ):
	
	message = 'Creating directory: ' + directory
	try:
		os.makedirs(directory)
		log.write(log.INFO, message)
	
	except OSError as exc:
			if exc.errno == errno.EEXIST:
				log.write(log.WARNING, 'directory "' + directory + '" already exists')
				pass
			else: 
				raise

	return

def create_file( filename ):
	
	message = 'Creating file: ' + filename
	_file = open( filename , 'w')
	_file.write('')
	_file.close()
	log.write(log.INFO, message)
	

#----------------------------------------------#
#		  Find the indeces for iteration       #
#----------------------------------------------#
def find_iteration_indeces( folder_name ):
	
	index_list = []

	# iterate, looking for positions of the # sign
	for idx in xrange( 0, len(folder_name)):
		if folder_name[idx] == '#':
			index_list.append(idx)
	
	return index_list

				
def recurse_create_folders( dir_name, indeces, starts, counts, zpad,  current = [] ):

	# create current position if it doesn't exist
	if current == []: 
		current = [0]*len(indeces)
	
	cnt = 0
	meterset = False
	file_list = []
	while True:

		# compute the actual filename
		filename = dir_name[:indeces[0]]

		for idx in xrange(0, len(indeces)-1):

			# add digit
			filename +=  str(current[idx]+starts[idx]).zfill(zpad)
			
			# add remainder
			filename += dir_name[ indeces[idx]+1 : indeces[idx+1]]
	   	
		# add remaining item
		filename += str(current[-1]+starts[-1]).zfill(zpad) + dir_name[ indeces[-1]+1 : ]
		
		file_list.append(filename)

		# now that we have the filename, increment the counts
		for cidx in xrange( 1, len(current)+1):
				
			# increment counter
			current[-cidx] += 1

			# check for overflow
			if current[-cidx] >= counts[-cidx]:

				# if overflow exists, reset position and move on
				current[-cidx] = 0
				meterset = True
			else:
				meterset = False
				break
		# end of cidx in xrange( 1, len(current)+1) loop
		
		# check to see if all counts are full
		if meterset == True:
			break

	# End of while True loop

	return file_list

#----------------------------------------------#
#                Build Layout				   #
#----------------------------------------------#
def build_layout( layout, current_dir, spaces, options, node_stack ):
	
	# iterate through each child
	for child in layout.getchildren():
		
		# check to see if tag is folder
		if child.tag == 'folder':

			# we need to make sure that the folder does not iterate
			if child.attrib.get("multiple") == "true":

				#permutation-style directories
				if child.attrib.get("type") == "permutation":
					
					#############################################
					# process iterating directory requirements
					#############################################

					# get the folder name
					dir_name = child.attrib.get('name')

					# first we need to find how many points of iteration exist
					indeces = find_iteration_indeces( child.attrib.get('name') )
				
					# for each index, query its starting point and count
					starts = []
					counts = []
					for idx in xrange(1, len(indeces)+1):
						query_s_name = 'start' + str(idx)
						query_c_name = 'count' + str(idx)
						starts.append( int( str(child.attrib.get( query_s_name ))))
						counts.append( int( str(child.attrib.get( query_c_name ))))
				
					# call the iteration folder function to recursively create folders
					zpad = int(child.attrib.get('zero_pad'))
					dir_names = recurse_create_folders( dir_name, indeces, starts, counts, zpad )
				
					# create directories and also enter using build
					for d in dir_names:
						
						dir_name = current_dir + '/' + d
						
						# create directory
						create_directory( dir_name )
	
						# call build_layout
						build_layout( child, dir_name, spaces + 3, options, node_stack + [child] )
				

				# counter-style directories
				if child.attrib.get("type") == "counter":
					
					# The key to this is managing a set of global counters
					# - start by looking the counter which corresponds to this tag type, 
					#   if it does not exist, then create it and manage its location
					
					cnt_found = False
					counter_idx = 0
					
					for pos in xrange( 0, len( options.global_counter)):
						
						# if the counter matches the directories' name, then get the index of the counter
						if options.global_counter[pos][0] == child.attrib.get('name'):
							counter_idx = pos
							cnt_found  = True
							break;

					# if the counter is not found, then create the counter
					if cnt_found == False:
						options.global_counter.append( [child.attrib.get('name'), 0] )
						counter_idx = len(options.global_counter)-1
					
					# now we have the counter, lets create the filenames
					# first we need to find how many points of iteration exist
					indeces = find_iteration_indeces( child.attrib.get('name') )
					
					# now lets find the start positions for each
					starts = []
					count  = int(str(child.attrib.get("count_per_dir")))
					zero_pad = int(str(child.attrib.get("zero_pad")))
					
					for idx in xrange(1, len(indeces)+1):
						query_s_name = 'start' + str(idx)
						starts.append( int( str(child.attrib.get( query_s_name ))))

					# given the starting position of the counter, start iterating
					for x in xrange( 0, count ):

						# build filename
						filename = child.attrib.get('name')[0:indeces[0]]

						# append counter value and increment
						filename += str(options.global_counter[counter_idx][1]).zfill(zero_pad)
						options.global_counter[counter_idx][1] += 1

						# append remaining string
						filename += child.attrib.get('name')[indeces[0]+1 :]
						

						# print directory name to create
						dir_name = current_dir + '/' + filename
						create_directory( dir_name )

						build_layout( child, dir_name, spaces+3, options, node_stack + [child] )
				
				# GS2 date-style directories
				if child.attrib.get("type") == "date":

					# grab relevant day, month, year info
					day   = child.attrib.get('day')
					month = child.attrib.get('month')
					year  = child.attrib.get('year')

					# build filename
					filename = str(month).zfill(2) + str(day).zfill(2) + str(year).zfill(4)
					dir_name = current_dir + '/' + filename
					 
					# create directory and pass on
					create_directory( dir_name )

					build_layout( child, dir_name, spaces+3, options, node_stack + [child] )

					
			else:
				# establish the new filename
				dir_name = current_dir + '/' + child.attrib.get('name')

				# create the directory
				create_directory(dir_name)
				
				# call function again on this folder
				build_layout( child, current_dir + '/' + child.attrib.get('name'), spaces + 3, options, node_stack + [child] )
		
		# end of if child.tag == 'folder'

		if child.tag == 'file':

			# Check for gs2 tacid
			if child.attrib.get('type') == 'tacid':
				
				# get the acquisition date
				acq_date = ''
				yearIMG  = 0
				monthIMG = 0
				dayIMG   = 0
				hourIMG  = 0
				minIMG   = 0

				# first search through the node stack for the date node
				if node_stack[-3].tag == 'folder' and node_stack[-3].attrib.get('name') == 'date':
													
					acq_date = node_stack[-3].attrib.get('day').zfill(2) + options.months[int(node_stack[-3].attrib.get('month'))] + node_stack[-3].attrib.get('year').zfill(4)[2:]
					yearIMG  = int(node_stack[-3].attrib.get('year'))
					monthIMG = int(node_stack[-3].attrib.get('month'))
					dayIMG   = int(node_stack[-3].attrib.get('day'))
				
				else:
					raise Exception("ERROR: Invalid directory format")

				# grab the minute and hour values
				hourIMG = int(node_stack[-2].attrib.get('name'))
				minIMG  = int(os.path.split(current_dir)[1])

				# Compute the product number
				camera_id = os.path.dirname(os.path.dirname(os.path.dirname(current_dir)))[-3:]
				local_product_number = options.global_camera_type + camera_id
				
				# now iterate through every possible frame in that minute, will be 60 seconds * global_hz
				for sec in xrange(0, 60):
					for tic in xrange( 0, options.global_hz):
						
						comp_global_time = 0
						comp_local_time  = 0

						# Compute the time differences between the current image and the global image
						magnifier = 1  					#No magnification
						comp_global_time += 0*magnifier
						comp_local_time  += tic*magnifier

						magnifier = options.global_hz			#2 ticks per second
						comp_global_time += 0*magnifier
						comp_local_time  += sec*magnifier

						magnifier *= 60					#60 seconds per minute
						comp_global_time += options.global_start_min * magnifier
						comp_local_time  += minIMG * magnifier

						magnifier *= 60					#60 minutes per hour
						comp_global_time += options.global_start_hour * magnifier
						comp_local_time	 += hourIMG * magnifier

						magnifier *= 24 				#24 hours per day
						comp_global_time += options.global_start_day  * magnifier
						comp_local_time  += dayIMG * magnifier

						# TODO: Compensate for which month it is
						magnifier *= 30					# This many days per month
						comp_global_time += options.global_start_month * magnifier
						comp_local_time  += monthIMG * magnifier

						magnifier *= 12                 # 12 months per year
						comp_global_time += options.global_start_year  * magnifier
						comp_local_time  += yearIMG * magnifier

						# compute scene difference
						scene_diff = comp_local_time - comp_global_time
						
						# NOTE:  For now, I am going to hard code the producer_serial_number to 0. The
						#        main idea is that it is supposed to provide overflow if the scene_number
						#        exceeds the 5 digits its actually assigned.  
						local_scene_number = str(scene_diff).zfill(5)
						local_producer_serial_number = ("%x" % 289).zfill(3)

						# compute the production datim 
						# create the current datetime
						date_obj = datetime.datetime( yearIMG, monthIMG, dayIMG, hourIMG, minIMG, sec, int(1E6)*tic/options.global_hz)
						local_production_datim = ("%x" % int(time.mktime(date_obj.timetuple()))).zfill(8).upper()
						
						tacid_name = ''
						
						# Append acqusition date to tacid
						tacid_name += acq_date 
						tacid_name += options.global_program_code 
						tacid_name += options.global_sortie_number 
						tacid_name += local_scene_number 
						tacid_name += options.global_producer_code
						tacid_name += local_product_number
						tacid_name += options.global_project_code
						tacid_name += options.global_replay_code
						tacid_name += local_producer_serial_number
						tacid_name += local_production_datim
						tacid_name += options.global_extension
					
						# create the actual tacid file
						create_file( current_dir + '/' + tacid_name )


		# end of if child.tag == 'file':

	# end of for child in layout.getchildren()


	return layout


#----------------------------------------------#
#                   Main Driver				   #
#----------------------------------------------#
def main():
	"""
	Main driver for the simulator program.  This will accept an xml file
	by default 'simulator.xml' and build a image directory with dummy values
	based on it. 
	"""

	#Open Configuration File and extract variables
	options = Options( sys.argv[1:] )
	
	global log 
	log = Logger( options )

	try:
		pass
		# recusively enter the nodes for layout and start building the tree
		build_layout( options.layout, '.', 0, options, [] )

		# close the log file
	except:
		log.write(log.MAJOR, sys.exec_info())

	finally:
		log.close()


if __name__ == "__main__":
	main()

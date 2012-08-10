#! /usr/bin/env python
import sys, os, errno, xml.etree.ElementTree as xml

global_counter = []

#####################################
#             TACID Parser			#
#####################################
class TACID:
	input_string     = []
	cor_input        = []
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
		self.cor_input = imgname

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

#----------------------------------------------#
#				Create Directory               #
#----------------------------------------------#
def create_directory( directory ):
	
	try:
		os.makedirs(directory)
	
	except OSError as exc:
			if exc.errno == errno.EEXIST:
				pass
			else: 
				raise

	return


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
def build_layout( layout, current_dir, spaces ):
	
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
						print ' '*spaces + '-> ' + d
						build_layout( child, dir_name, spaces + 3 )
				

				# counter-style directories
				if child.attrib.get("type") == "counter":
					
					# The key to this is managing a set of global counters
					# - start by looking the counter which corresponds to this tag type, 
					#   if it does not exist, then create it and manage its location
					
					cnt_found = False
					counter_idx = 0
					
					for pos in xrange( 0, len(global_counter)):
						
						# if the counter matches the directories' name, then get the index of the counter
						if global_counter[pos][0] == child.attrib.get('name'):
							counter_idx = pos
							cnt_found  = True
							break;

					# if the counter is not found, then create the counter
					if cnt_found == False:
						global_counter.append( [child.attrib.get('name'), 0] )
						counter_idx = len(global_counter)-1
					
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
						filename += str(global_counter[counter_idx][1]).zfill(zero_pad)
						global_counter[counter_idx][1] += 1

						# append remaining string
						filename += child.attrib.get('name')[indeces[0]+1 :]
						
						print ' '*spaces + '-> ' + filename

						# print directory name to create
						dir_name = current_dir + '/' + filename
						create_directory( dir_name )

						build_layout( child, dir_name, spaces+3)
				
				# GS2 date-style directories
				if child.attrib.get("type") == "date":

					# grab relevant day, month, year info
					day   = child.attrib.get('day')
					month = child.attrib.get('month')
					year  = child.attrib.get('year')

					# build filename
					filename = str(month).zfill(2) + str(day).zfill(2) + str(year).zfill(4)
					dir_name = current_dir + '/' + filename
					 
					print ' '*spaces + '-> ' + filename
					
					# create directory and pass on
					create_directory( dir_name )

					build_layout( child, dir_name, spaces+3)

					
			else:
				# establish the new filename
				dir_name = current_dir + '/' + child.attrib.get('name')
				print ' '*spaces + '-> ' + child.attrib.get('name') 

				# create the directory
				create_directory(dir_name)
				
				# call function again on this folder
				build_layout( child, current_dir + '/' + child.attrib.get('name'), spaces + 3)
		
		# end of if child.tag == 'folder'

		if child.tag == 'file':

			# Check for gs2 tacid
			if child.attrib.get('type') == 'tacid':

				# since this is a tacid, we need to build that list of information


				pass


		# end of if child.tag == 'file':

	# end of for child in layout.getchildren()



#----------------------------------------------#
#                   Main Driver				   #
#----------------------------------------------#
# Parse Command-Line Arguments
args = sys.argv[1:]
config_filename = "simulator.xml"

# iterate through each argument
for item in args:
	if item[:6] == '-file=':
		config_filename = item[6:]
	else:
		raise Exception("ERROR: unsupported option")


#Open Configuration File and Build XML Tree
root = xml.parse(config_filename)

# recusively enter the nodes for layout and start building the tree
layout = root.find('layout')

build_layout( layout, '.', 0 )




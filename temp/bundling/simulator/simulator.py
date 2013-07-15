#! /usr/bin/env python
import sys, os, errno, xml.etree.ElementTree as xml, datetime, time

NEWLINE = '\n'

##################################
#      Configuration Options     #
##################################

BASE_PATH='dsu'

CAMERA_TYPE='EO'

NUM_CAM_DIRS=368

START_SLICE=1

# CAL COP NUC
COLLECT_PATH=CAMERA_TYPE + '_CAL_20120818080808'

YR=12
MO='AUG'
DY=18

START_TIME=[8,0]
END_TIME=[8,10]

N_TOGGLE=False
R_TOGGLE=False

PROGRAM_CODE = '9Z'
SORTIE_NUMBER = '00'
PRODUCER_CODE = 'ZX'
PROJECT_CODE = 'GS'
REPLAY_CODE  = '000'
PRODUCER_SERIAL_NUMBER = 'AAA'
PRODUCTION_DATIM = 'EEEEEEEE'
EXT='.ntf'

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
				print 'odd'
				raise

	return

def create_file( filename ):
	
	_file = open( filename , 'w')
	_file.write('')
	_file.close()
	

#---------------------------------------------#
#-             CREATE THE TACIDs             -#
#---------------------------------------------#
def build_layout_10( BASE_PATH, _slice, collect, camera, hour, minute ):
	
	# Get the hour and minute difference difference
	hdiff =  hour  - START_TIME[0]
	mdiff = minute - START_TIME[1]
		
	time_diff = (hdiff*(120*60)) + (mdiff*120)

	# We need to create 60 images per camera directory, but split between collects 0/1 and 2/3
	for x in xrange(0, 60):
		
		TACID = ''

		# Add the date
		TACID += str(DY).zfill(2) + MO + str(YR).zfill(2)
		
		# Add the Program Code
		TACID += PROGRAM_CODE
		
		# Add the Sortie Number
		TACID += SORTIE_NUMBER

		# The scene number will be the current x in addition to the hour and minute combo
		scene_number = 0
		if collect == 0 or collect == 2:
			scene_number = 2*x + time_diff
		else:
			scene_number = 2*x + time_diff + 1

		TACID += str(scene_number).zfill(5)
		
		# Add the Producer Code
		TACID += PRODUCER_CODE
		
		# Add the Product Number
		TACID += 'G' + CAMERA_TYPE + camera
		
		# Add the Project Code
		TACID += PROJECT_CODE
		
		# Add the Replay Code
		TACID += REPLAY_CODE

		# Add the serial number
		TACID += PRODUCER_SERIAL_NUMBER
		
		# Add the production datic
		TACID += PRODUCTION_DATIM

		# Add the extension
		TACID += EXT

		# Create the file
		create_file( BASE_PATH + '/' + TACID )


#---------------------------------------------#
#-         BUILD THE MINUTE DIRECTORY        -#
#---------------------------------------------#
def build_layout_09( BASE_PATH, _slice, collect, camera, hour ):

	ran = [0, 60]
	if   hour == START_TIME[0]:
		ran[0] = START_TIME[1]
	if   hour == END_TIME[0]:
		ran[1] = END_TIME[1]+1

	for x in xrange( ran[0], ran[1]):
		cdir = BASE_PATH + '/' + str(x).zfill(2)
		create_directory(cdir)
		build_layout_10( cdir, _slice, collect, camera, hour, x )	

#---------------------------------------------#
#-          BUILD THE HOUR DIRECTORY         -#
#---------------------------------------------#
def build_layout_08( BASE_PATH, _slice, collect, camera ):

	# Create the hours
	for x in range( START_TIME[0], END_TIME[0]+1):
		cdir = BASE_PATH + '/' + str(x).zfill(2)
		
		create_directory( cdir )
		build_layout_09( cdir, _slice, collect, camera, x )


#---------------------------------------------#
#-          BUILD THE DATE DIRECTORY         -#
#---------------------------------------------#
def build_layout_07( BASE_PATH, _slice, collect, camera ):
	
	cdir = BASE_PATH + '/' + str(YR).zfill(2) + MO + str(DY).zfill(2)
	create_directory( cdir )
	build_layout_08( cdir, _slice, collect, camera )

#---------------------------------------------#
#-   BUILD THE NUMBERED CAMERA DIRECTORIES   -#
#---------------------------------------------#
def build_layout_06( BASE_PATH, _slice, collect ):
	
	cflag = 0
	if collect >= 2:
		cflag = 1

	DIR_RESULT = (46*(_slice-1)) + 23*cflag
	
	# for each camera up to the max, set the camval and build the directory
	for x in xrange(0, 23):
		CAMVAL = str(DIR_RESULT + x).zfill(3)

		# There will be 23 images per half slice
		cdir = BASE_PATH + '/cam' + CAMVAL 
		
		create_directory( cdir )
		
		# Create the date folder
		build_layout_07( cdir, _slice, collect, CAMVAL )
		
		print cdir

#---------------------------------------------#
#-      BUILD THE COLLECTION DIRECTORIES     -#
#---------------------------------------------#
def build_layout_05( BASE_PATH, _slice, collect ):

	cdir = BASE_PATH + '/' + COLLECT_PATH
	
	create_directory( cdir )

	build_layout_06( cdir, _slice, collect)

#----------------------------------------------#
#         BUILD THE NITF/RAW DIRECTORIES       #
#----------------------------------------------#
def build_layout_04( BASE_PATH, _slice, collect ):
	
	ndir = BASE_PATH + '/nitf'
	rdir = BASE_PATH + '/raw'

	create_directory( ndir )
	create_directory( rdir )

	build_layout_05( ndir, _slice, collect )
	#build_layout_05( rdir, _slice, collect )

#-----------------------------------------#
#-     BUILD THE CAMERA DIRECTORIES      -#
#-----------------------------------------#
#def build_layout_03( BASE_PATH, _slice, collect ):
#
#	cdir = BASE_PATH + '/camera'
#	create_directory( cdir )
#	
#	build_layout_04( cdir, _slice, collect )


#----------------------------------------------#
#-     BUILD THE SLICE COLLECT DIRECTORIES    -#
#----------------------------------------------#
def build_layout_02( BASE_PATH ):
	
	# Build the slice directories
	for x in xrange(START_SLICE,9):
		for y in xrange(0,4):
			cdir = BASE_PATH + '/slice'+str(x)+'-collect'+str(y)
			create_directory( cdir )
			
			build_layout_04( cdir, x, y )


#-----------------------------------------#
#-     BUILD THE BASE DSU DIRECTORY      -#
#-----------------------------------------#
def build_layout_01( ):

	# Build base directory
	create_directory( BASE_PATH )

	build_layout_02( BASE_PATH )


#----------------------------------------------#
#         Build Some Example Cal Files         #
#----------------------------------------------#
def build_cal_files( ):
	"""
	Create the file structure for cal files
	"""

	# create the directory for housing cal files
	CAL_FILE_BASE="/central-data/cal_files";
	create_directory( CAL_FILE_BASE )

	# create some dummy files
	create_file( CAL_FILE_BASE + "/eo_cal_file_1001.xml");
	create_file( CAL_FILE_BASE + "/eo_cal_file_1002.xml");
	create_file( CAL_FILE_BASE + "/ir_cal_file_1001.xml");
	create_file( CAL_FILE_BASE + "/ir_cal_file_1002.xml");



#----------------------------------------------#
#                   Main Driver				   #
#----------------------------------------------#
def main():
	"""
	Main driver for the simulator program.  This will accept an xml file
	by default 'simulator.xml' and build a image directory with dummy values
	based on it. 
	"""

	# recusively enter the nodes for layout and start building the tree
	build_layout_01( )


	#  Create the file structure for the cal files
	build_cal_files( )


if __name__ == "__main__":
	main()

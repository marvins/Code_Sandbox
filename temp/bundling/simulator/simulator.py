#! /usr/bin/env python
import os, errno

def create_directory( directory ):
	
	try:
		os.makedirs(directory)
	
	except OSError as exc:
			if exc.errno == errno.EEXIST:
				pass
			else: 
				raise

	return


#builds a sample layout

# Build base directory
basedir = "dsu"
create_directory( basedir )
current = 0

# Build collect directories
collectdir = ["slice","-collect"]
actual = "";
for x in range(1,8+1):
	for y in range( 0, 4):
		
		actual = basedir + "/" + collectdir[0] + str(x) + collectdir[1] + str(y)
		create_directory( actual )

		#create camera raw directories
		actual += "/camera/raw"
		create_directory( actual )
		
		for z in range( 0, 12 ):
			camdir = actual + "/cam" + str(current).zfill(3);
			create_directory( camdir );



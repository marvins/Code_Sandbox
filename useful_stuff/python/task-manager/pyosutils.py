import os, sys

def ls( dirname, filters = [] ):

	output = []

	for filename in os.walk( dirname ):
	
		for file in filename[2]:

			# Check for extension filters
			for filter in filters:
				if filter in file:
					output.append( dirname + '/' + file)
	
	return output

def rm( filename ):
	
	os.remove(filename)



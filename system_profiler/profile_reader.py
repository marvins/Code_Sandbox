#! /usr/bin/env python

import sys

def main( ):

	# Grab the log filename
	filename=sys.argv[1]

	# Open the file and start reading lines
	FILE = open( filename, 'r' )

	# Read each line
	for line in FILE:
		
		# Strip the end and front garbage
		line = line.strip()
		
		print line


	FILE.close()


if __name__ == "__main__":
	main()

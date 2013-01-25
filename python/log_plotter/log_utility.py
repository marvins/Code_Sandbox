#! /usr/bin/env python

import os, sys, re, datetime

p = re.compile("[\s;]")


###################################################
#    Convert the timestamp into an epoch time     #
###################################################
def time2epoch( datestr, timestr ):

	# Split the date by the slash
	date_parts = datestr.split("/")

	# Split the time by the colon
	time_parts = timestr.split(":")

	# get the mon, day, and year
	mon = int(date_parts[0])
	day = int(date_parts[1])
	yea = int(date_parts[2])

	# get the hours, min, secs
	hr  = int(time_parts[0])
	mn  = int(time_parts[1])
	sc  = float(time_parts[2])
	
	sec = int(sc)
	milsec= int((sc - sec)*1E6)
	
	# return the datetime object
	return datetime.datetime( 2000 + yea, 
	                 mon,
					 day,
					 hr,
					 mn,
					 sec,
					 milsec ).isoformat()





############################################
#      Command Line Options Container      #
############################################
class Options:
	
	log_filename = ""
	csv_filename = ""
	component    = ""
	keys         = []
	
	def print_options(self):
		
		print 'Log Filename: ', self.log_filename
		print 'Output File : ', self.csv_filename
		print 'Component   : ', self.component
		print 'Keys        : ', self.keys


###################################################
#               Write Data to CSV                 #
###################################################
def write_csv( data, options ):

	# Open a file for output
	csvfile = open( options.csv_filename, 'w' )
	
	
	# Add each data line to the file
	for x in xrange(0,len(data)):
		for y in xrange(0,len(data[x])):
			csvfile.write(str(data[x][y]) + ", ")
		csvfile.write("\n")

	# Close the csv file
	csvfile.close()


############################################
#       Parse key/value pairs from the     #
#       current entry.                     #
############################################
def parse_entry( entry, options ):
	
	# Split the entry according to the regex provided up top
	elements = p.split(entry)
	
	# Parse out the timestamp value
	time = time2epoch( elements[0], elements[1])

	# Crop off the time parts
	elements = elements[2:]

	# Create a Key
	row_item = [time]
	for x in xrange(0,len(options.keys)):
		row_item.append([""])

	# Iterate through each key/value pair and extract the value.
	# If it is missing, leave it blank
	for element in elements:
		for k in xrange(0,len(options.keys)):
			
			# If the key is in the element list, add it to the row item
			if options.keys[k] in element:
				row_item[k+1] = element.split("=")[1]
	
	
	return row_item
			


############################################
#          Open and Parse Logfile          #
############################################
def open_and_parse_logfile( options ):
	"""
	Open the file and read the log contents
	"""
	
	# Make sure the logfile exists
	if os.path.exists( options.log_filename ) == False:
		raise Exception("ERROR: " + options.log_filename + " does not exist.")
	
	
	# initialize the first row
	top_row = ['Timestamp']
	for key in options.keys:
		top_row.append(key)
	
	data = []
	data.append(top_row)
	

	# Create the file
	logfile = open(options.log_filename, 'r')

	# Grab each logfile line
	current_entry = ""
	for line in logfile:
		
		# Append line to current log entry
		current_entry += line.strip()
		
		# If the line has the correct ending 
		if "@E" in current_entry:
			
			# Test for component 
			if options.component in current_entry:

				# Since the component exists, lets pull out each key
				value = parse_entry( current_entry, options )
				
				data.append(value)

			# Clear current entry
			current_entry = ""


	# Close the file
	logfile.close()
	
	# return the data
	return data
	

############################################
#        Parse Command-Line Options        #
############################################
def parse_command_line_options( args ):
	
	# Get rid of the first arg (program name)
	args = args[1:]

	# Create an output object to return
	options = Options()

	# iterate through each option and take appropriate action
	while len(args) > 0:

		# Check for the log filename
		if args[0][0:5] == "-log=":
			options.log_filename = args[0][5:]
			args = args[1:]
		
		# Check for the output csv filename
		elif args[0][0:5] == "-csv=":
			options.csv_filename = args[0][5:]
			args = args[1:]
		
		# Check for the desired component
		elif args[0][0:6] == "-comp=":
			options.component = args[0][6:]
			args = args[1:]

		# Check for the keys
		elif args[0][0:6] == "-keys=":
			
			# Pop items until we run out
			
			# if there is a key next to the equal, grab it
			if len(args[0][6:]) > 0:
				options.keys.append(args[0][6:])
				args = args[1:]
			
			# Find the remaining keys
			while True:
				if len(args) <= 0:
					break
				elif args[0][0] == "-":
					break
				else:
					options.keys.append(args[0])
					args = args[1:]
		
		# If we don't get something expected, then cancel
		else:
			raise Exception("ERROR: Unknown flag " + args[0])


	# return to main
	return options

############################################
#                 MAIN                     #              
############################################
def main( ):
	""" 
	Main driver for the log parsing tool
	"""

	# Parse command-line options
	options = parse_command_line_options( sys.argv )
	
	# Parse logfile
	data = open_and_parse_logfile( options )
	
	# write to csv
	write_csv( data, options )

if __name__ == "__main__":
	main()

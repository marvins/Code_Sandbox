#! /usr/bin/env python

import sys, re, datetime, numpy as np, matplotlib as mpl, matplotlib.pyplot as plt

p = re.compile("[\s;]")

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
					 milsec )


def split_and_parse_entry( current, graphType, compType ):

	fst = ""
	snd = ""
	snd_set = False

	# Split the line by spaces
	elements = p.split(current)
	
	# Pull out the time
	fst = time2epoch( elements[0], elements[1]) 

	elements = elements[2:]
	# Iterate through the list, looking for our plot items
	for element in elements:
		
		if graphType in element:
			data = element.split("=")[1]
			snd_set = True
			snd     = data
		
		if snd_set == True:
			return (fst, snd)

	raise Exception("ERROR: data not found")


def read_log( filename, graphType, compType ):
	"""
	Open the file and read the log contents
	"""

	# Create storage for log data
	logdata = []

	# create logfile
	logfile = open(filename, 'r')
	
	# Grab each logfile line
	current_entry = ""
	for line in logfile:
		
		# Append line to current log entry
		current_entry += line.strip()

		# If the line has the correct ending 
		if "@E" in current_entry:
			
			# Test for comp and graph values
			if graphType in current_entry and compType in current_entry:
				
				# split the line into plottable data
				plot_data = split_and_parse_entry( current_entry, graphType, compType )

				# Add data to logdata
				logdata.append(plot_data)

			# Clear current entry
			current_entry = ""



	logfile.close()

	return logdata



############################################
#                 MAIN                     #              
############################################
def main( ):
	""" 
	Main driver for the log parsing tool
	"""

	# Grab filename
	filename = sys.argv[1]
	
	# Grab X tag
	compType = sys.argv[2]

	# Grab Y tag
	graphType= sys.argv[3]

	# Read the log
	data = read_log( filename, graphType, compType )
	
	# Parse out the configuration file
	x_axis = []
	y_axis = []
	
	for line in data:
		x_axis.append(line[0])
		y_axis.append(line[1])

	x = np.array(x_axis)
	y = np.array(y_axis)
	
	yscale = float(max(y)) * .05 
	ymin = 0
	ymax = float(max(y)) + yscale

	# plot the graph
	plt.plot( x, y );
	
	# set the temp limits
	plt.ylim( ymin, ymax )

	# set the grid on
	plt.grid(True)
	
	# create labels for axis
	plt.xlabel("Time")
	plt.ylabel("Temperature [F]")

	# show the graph
	plt.savefig( "output.pdf" )


if __name__ == "__main__":
	main()

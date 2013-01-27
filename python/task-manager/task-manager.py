#!/usr/bin/env python

import curses, sys, os

#   Class to manage arguments and configuration details
class Options:
	
	argname = ""

	def __init__( self, args ):

		#  Make sure arguments are supplied
		if len(args) > 0:
			self.argname=args[0]	
		


	def usage( self ):
		
		print( "Usage:")


#  Function to manage and initialize curses
def init_curses( ):
	screen = curses.initscr()
	curses.noecho()
	curses.cbreak()
	screen.keypad(1)
	return screen;

#  Function to clean up curses
def close_curses( screen ):
	curses.nocbreak()
	screen.keypad(0)
	curses.echo()
	curses.endwin()
	

#   Management Console
def management_console( screen, options ):

	#  Start a loop to show the UI
	while True:

		#  Clear the console
		screen.refresh()





		# Grab user input






#  Main Function
def main( ):
	
	#  Process command-line arguments
	options = Options( sys.args )

	#  Start UI
	try:

		#  Initialize Curses
		screen = init_curses( )
		
		#  Go to management console
		management_screen( screen, options )


	finally:
		
		#  Close Curses
		close_curses( screen )


if __name__ == "__main__":
	main()

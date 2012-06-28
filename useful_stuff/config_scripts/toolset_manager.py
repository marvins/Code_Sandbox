#! /usr/bin/env python

import curses

# initialize curses
main_window = curses.initscr()


def main_menu( ):
	
	# create loop mechanism for menu
	end_menu = False
	while end_menu != True:
		

		# draw main menu
		main_window.addstr( 1, 1, "GeoImage Toolset Manager", curses.A_STANDOUT )

		main_window.refresh()
		
		# get character for user input
		input = main_window.getch()
		main_window.nodelay(1)

		if input == ord('q'):
			end_menu = True
		



		

def main():
	
	
	# some useful flags
	curses.noecho()
	curses.cbreak()
	main_window.keypad(1)
	
	# pass to main menu
	main_menu( )

	# end curses session
	curses.echo()
	curses.nocbreak()
	main_window.keypad(0)
	curses.endwin()



if __name__ == "__main__":
	main()

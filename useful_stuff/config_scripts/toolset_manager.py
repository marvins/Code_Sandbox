#! /usr/bin/env python

import curses

# initialize curses
main_window = curses.initscr()


def main_menu( ):
	
	# create loop mechanism for menu
	cursor_pos = 2
	menu_arr = []
	menu_arr = menu_arr + ["Manage GDAL"]
	menu_arr = menu_arr + ["Manage OpenCV"]
	menu_arr = menu_arr + ["Exit Program"]
	end_menu = False
	while end_menu != True:
		

		# draw main menu
		main_window.addstr( 1, 1, "GeoImage Toolset Manager" )
		
		for y in range( 0, len( menu_arr)):
			if y == cursor_pos:
				main_window.addstr( y+3, 1, menu_arr[y], curses.color_pair(1))
			else:
				main_window.addstr( y+3, 1, menu_arr[y], curses.color_pair(2))

		main_window.refresh()
		
		# get character for user input
		inputKey = main_window.getch()
		main_window.nodelay(1)

		if inputKey == ord('q'):
			end_menu = True
		elif inputKey == curses.KEY_UP:
			cursor_pos -= 1
			if cursor_pos < 0:
				cursor_pos = len(menu_arr)-1
		elif inputKey == curses.KEY_DOWN:
			cursor_pos += 1
			if cursor_pos >= len( menu_arr ):
				cursor_pos = 0



		

def main( args ):
		
	
	# some useful flags
	curses.noecho()
	curses.cbreak()
	main_window.keypad(1)
	
	# manage colors
	curses.start_color()	
	curses.init_pair( 1, curses.COLOR_BLUE , curses.COLOR_BLACK)
	curses.init_pair( 2, curses.COLOR_BLACK, curses.COLOR_BLUE )

	
	# pass to main menu
	main_menu( )

	# end curses session
	curses.nocbreak()
	main_window.keypad(0)
	curses.echo()
	curses.endwin()
	
	return 1


if __name__ == "__main__":
	curses.wrapper(main)

import curses

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
	

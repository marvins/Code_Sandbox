#!/usr/bin/env python

import curses, sys, os, time
from pycursesutils import *
import pyosutils

class TaskManager:
	
	#  GUI Mode Flags
	GUI=True
	NO_GUI=False
	
	# Location of base configuration
	config_file = os.environ['HOME'] + '/.task-manager/options.cfg'
	
	# Location of tasks
	task_directory = os.environ['HOME'] + '/.task-manager/tasks'

	def read_config(self):
		
		# Check if the config file exists
		if os.path.exists( self.config_file ) == False:
			if os.path.exists( os.path.split(self.config_file)[0] ) == False:
				os.makedirs(os.path.split(self.config_file)[0])
			
			print("Task configuration does not exist.  Writing default to "+self.config_file)
			FILE = open(self.config_file, 'w')
			FILE.write('# Configuration File For Task Manager\n')
			FILE.write('TASK_DIRECTORY='+self.task_directory+'\n')
			FILE.close()
		
		# Check if the task directory exists
		if os.path.exists( self.task_directory ) == False:
			print("Task Directory does not exist. Writing default to "+self.task_directory)
			os.makedirs(self.task_directory)

		# Open the config file and parse all options
		FILE = open(self.config_file, 'r')
		for line in FILE.readlines():
			
			# Trim down the line
			line = line.strip()

			# ignore if empty
			if len(line) <= 0:
				continue
			
			# ignore if a comment
			if line[0] == '#':
				continue

			# Split by equal sign
			data = line.split('=')
		
			# Compare the items
			if data[0] == 'TASK_DIRECTORY':
				self.task_directory = data[1]
			else:
				raise Exception('Error: unknown config file options: ' + data[0] + '=' + data[1])


		FILE.close()




#   Create the global configuration items
program_data = TaskManager()



class Task:
	
	name=None
	groups=[]
	notes=""
	filename=""

	def __init__(self, args = []):
		
		# Iterate through the arguments, parsing them
		while len(args) > 0:
			
			carg = args[0]
			args = args[1:]
			
			if carg == '-name':
				self.name = args[0]
				args = args[1:]

			elif carg == '-groups':
				
				groups=args[0]
				args = args[1:]
				self.groups = groups.split(',')
				
			else:
				raise Exception("Unknown item: " + str(args))
	
	def __str__(self):
		
		output = "TASK\n"
		output += '  Name: ' + self.name + '\n'
		output += '  Groups: '
		for group in self.groups:
			output += group + ' '
		
		return output
	

	def write(self):
		
		FILE = open(self.filename, 'w')
		FILE.write( str(self) + '\n')
		FILE.close()


def readtask( filename):
	"""
	Read the current task from a file.
	"""
	#  Create a task and open the file
	task = Task()
	task.filename = os.path.basename(filename)
	FILE = open( filename, 'r' )
	
	#  Read each line
	noteOn = False
	for line in FILE.readlines():
		line = line.strip()
		
		#  Task Name
		if 'Name:' == line[:5]:
			task.name = line[5:]

		#  Task Groups
		elif 'Groups:' == line[:7]:
			task.groups = line[7:].split(',')

		#  Notes
		elif 'Notes:' == line[:6]:
			task.notes += line[6:]
			noteOn = True
		elif noteOn == True:
			task.notes += line
	
	FILE.close()
	
	return task


#   Class to manage arguments and configuration details
class Options:
	
	argname = ""
	exit_after_init = False
	args= []
	list_task_mode = False

	tasklist=[]

	def __init__( self, args ):

		#  Make sure arguments are supplied
		if len(args) > 0:
			self.argname=args[0]	
			args = args[1:]

			# If no more args exist, then continue to program
			if len(args) <= 0:
				pass
			
			elif args[0] == '-c':
				print 'creating task'
				
				# Remove the flag parameter
				args = args[1:]
				self.args = args
				create_task( TaskManager.NO_GUI, self )
				self.exit_after_init = True

			#  For deleting tasks
			elif args[0] == '-d':
				print 'deleting task'

			#  Importing task files into your program
			elif args[0] == '-i':
				print 'importing task'

			#  List all current tasks
			elif args[0] == '-l':
				self.list_task_mode = True
				self.exit_after_init = True

			#  Delete all tasks
			elif args[0] == '-x':
				print 'clearing tasks'
				clear_tasks( )

			else:
				raise Exception('Unknown command flag: ' + args[0])
		
		#  Finally, we need to load our task list
		tasklist = pyosutils.ls( program_data.task_directory, ['.task'])
		for task in tasklist:
			self.tasklist.append( readtask(task) )
	
		#  List the tasks
		if self.list_task_mode == True:
			self.list_tasks( )

	
	def usage( self ):
		
		print( "Usage:")

	def list_tasks( self ):
		
		print('Current Task List')
		for task in self.tasklist:
			sys.stdout.write( 'Name: ' + task.name + '\n')



def create_task( GUI_MODE, options, screen = None ):
	
	global program_data

	#  Create a new task
	task = Task()
	task.name = ""
	
	# Create a list of indeces to keep track of cursor progress
	cursors=[0]
	text = ['',[''],'']

	#  Create a task based on the input options
	if GUI_MODE == TaskManager.NO_GUI:
		
		# We assume that they already provided the arguments
		task = Task( options.args )

		# compute a filename
		task.filename = program_data.task_directory + '/TASK_' + str(int(time.mktime(time.gmtime()))) + '.task'
		
		task.write();	
	
	#  Print a new GUI
	else:	
		
		size = screen.getmaxyx()
		
		#  Current cursor position
		cursor = 0

		#  Current string cursor position
		cpos = [0,0,[0]]
		cnote = 0

		maxc   = 2
		noteheight = 1

		while True:
		
			# Clear the console
			screen.clear()

			# Print the header 
			screen.addstr( 1, 1, 'Task Management Console')
			screen.addstr( 2, 1, '     Create Task')
			screen.addstr( 3, 1, '-----------------------')
			
			#  Name
			if cursor == 0:
				screen.attron(curses.A_STANDOUT)
			
			screen.addstr( 4, 1, 'Name: ' + text[0])
			if cursor == 0:	
				screen.attroff(curses.A_STANDOUT)
			
			#  Groups
			if cursor == 1:
				screen.attron(curses.A_STANDOUT)
			
			screen.addstr( 5, 1, 'Groups: ' )

			if cursor == 1:	
				screen.attroff(curses.A_STANDOUT)
			
			#  Notes
			if cursor == 2:
				screen.attron( curses.A_STANDOUT)
			
			screen.addstr( 6, 1, 'Notes: ' )
			if cursor == 2:
				screen.attroff( curses.A_STANDOUT)
			for x in xrange(0,len(text[2])):
				screen.addstr( 7+x, 1, text[2][x] )

			screen.addstr(size[0]-3, 1, '-----------------------------')
			screen.addstr(size[0]-2, 1, '  Press any key to continue: ')
			screen.addstr(size[0]-1, 1, ' Options: Type in values, Up/Dn Arrows- Switch, ESC- exit')

			#  Print the menu
			screen.refresh( )

			# Grab user input
			selection = screen.getch()
	
			#  If the user wants to change items
			if selection == curses.KEY_UP:
				cursor -= 1
				if cursor < 0:
					cursor = maxc

			elif selection == curses.KEY_DOWN:
				cursor += 1
				if cursor >= maxc+1:
					cursor = 0

			# Enter text
			elif (selection >= ord('a') and selection <= ord('z') or (selection >= ord('A') and selection <= ord('Z'))):
				text[cursor][cnote] += chr(selection)
			elif selection == ord(' ') or selection == ord('.') or selection == ord(','):
				text[cursor][cnote] += chr(selection)
			
			elif selection == 10 and cursor == 2:
				text[cursor] += '\n'
				noteheight += 1
			
			#  Backspace
			elif selection == curses.KEY_BACKSPACE:
				text[curses] = text[curses][cpos[curses]-1] + text[curses][cpos[curses]+1]

			#  Save and exit
			elif selection == 27 and cursor == 2:
				cnote += 1
				text[curses].append('')
				return


def refresh_list( options ):

	# Get a list list of the files
	tasklist = pyosutils.ls( program_data.task_directory, ['.task'])
	for taskfile in tasklist:
		for task in options.tasklist:
			if not taskfile in task.name:
				self.tasklist.append( readtask(taskfile) )


def view_task( GUI_MODE, options, screen, cursor ):
	
	#  GUI Mode
	if GUI_MODE == TaskManager.GUI:
		
		while True:
			
			#  Print the screen
			screen.clear()

			#  Print the name
			screen.addstr( 1, 1, 'Task Management Console')
			screen.addstr( 2, 1, '     View Task')
			screen.addstr( 3, 1, '-----------------------')

			#  Print the Task Name
			screen.addstr( 4, 1, 'Name: ' + options.tasklist[cursor].name)
		
			#  Print the Groups
			cpos = 8
			screen.addstr( 5, 1, 'Groups: ')
			for group in options.tasklist[cursor].groups:
				screen.addstr( 5, cpos, group )
				cpos += len(group)
			
			#  Print Notes
			screen.addstr( 7, 1, 'Notes:')
			screen.addstr( 8, 1, options.tasklist[cursor].notes )

			#  Get character
			screen.refresh()
			selection = screen.getch()
			
			#  Exit window
			if selection == ord('Q') or selection == ord('q'):
				break


	#  Console Mode
	else:

		sys.stdout.write('Task Name: ' + options.tasklist[cursor])

	


def clear_tasks( ):
	"""
	Clear the task directory
	"""
	global program_data

	#  Find the task directory and get a list of contents
	files = pyosutils.ls( program_data.task_directory,  ['.task'])

	for file in files:
		pyosutils.rm( file )


####################################################################
#                    Management Header Screen                      #
####################################################################
def management_header( screen, options, cursor ):
	
	#  Grab the current window size
	size = screen.getmaxyx()
	bot = 3+len(options.tasklist)

	# Print the top window
	screen.addstr( 1, 1, 'Task Management Console')
	screen.addstr( 2, 1, '-----------------------')
	screen.addstr( 3, 1, 'Current Open Tasks')
	screen.noutrefresh()

	# Print all open tasks
	for x in xrange( 0, len(options.tasklist)):
		if x == cursor:
			screen.attron( curses.A_BOLD )
		
		screen.addstr( 3+x, 1, str(x) + ': ' + str(options.tasklist[x].name) ) 
		
		if x == cursor:
			screen.attroff( curses.A_BOLD )
	
	#  Print a message if no tasks exist
	if len( options.tasklist ) <= 0:
		screen.addstr( 5, 1, "No Tasks Available" )
		bot += 3

	# Print the footer
	screen.addstr( bot + 1, 1, '-----------------------------')
	screen.addstr( bot + 2, 1, '  Press any key to continue: ')
	screen.addstr( bot + 3, 1, ' Options: Q/ECS-Exit Program, C-Create Task, D-Delete Task, V-View Tasks')
	screen.addstr( bot + 4, 1, '          Up/Dn Arrows- Navigate,  Enter-View, R-Refresh List')
	screen.refresh()


####################################################################
#                      Management Console UI                       #
####################################################################
def management_console( screen, options ):

	#  Set the screen delay
	screen.nodelay(True)
	screen.timeout(2000)
	cursor = 0

	#  Start a loop to show the UI
	while True:

		#  Clear the console
		screen.clear()

		management_header( screen, options, cursor )

		
		# Compute the position of the cursor
		pos = 5+len(options.tasklist)
		if len(options.tasklist) <= 0:
			pos += 3

		# Grab user input
		screen.move(pos,30)
		screen.refresh()
		selection = screen.getch()
		

		###################################
		#       Process User Input
		###################################
		#  Exit Console
		if selection == 27 or selection == ord('Q') or selection == ord('q'):
			screen.addstr( pos,30,'Exiting Program')
			screen.refresh()
			time.sleep(0.5)
			break

		#  Create Task
		elif selection == ord('C') or selection == ord('c'):
			create_task( TaskManager.GUI, options, screen )
		
		#  View Task
		elif selection == ord('V') or selection == ord('v') or selection == 13:
			view_task(  TaskManager.GUI, options, screen, cursor );
		
		#  Refresh Task List
		elif selection == ord('R') or selection == ord('r'):
			refresh_list( )

		#  Arrow Up Key
		elif selection == curses.KEY_UP:
			cursor -= 1
			if cursor < 0:
				cursor = len(options.tasklist) - 1

		#  Arrow Down Key
		elif selection == curses.KEY_DOWN:
			cursor += 1
			if cursor >= len(options.tasklist):
				cursor = 0




#  Main Function
def main( ):
	
	#  Process command-line arguments
	options = Options( sys.argv )

	#  Read System Configuration
	global program_data
	program_data.read_config();
	
	#  Exit the program to prevent the GUI
	if options.exit_after_init == True :
		return 0;

	#  Initialize Curses
	screen = init_curses( )
	
	#  Start UI
	try:

		
		#  Go to management console
		management_console( screen, options )


	finally:
		
		#  Close Curses
		close_curses( screen )
	
	return 0


if __name__ == "__main__":
	code = main()
	exit(code)


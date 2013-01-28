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
	
	def read(filename):
		
		# Create a blank task
		task = Task()

		FILE = open(self.filename, 'r' )
		
		FILE.close()

		return task


	def write(self):
		
		FILE = open(self.filename, 'w')
		FILE.write( str(self) + '\n')
		FILE.close()


#   Class to manage arguments and configuration details
class Options:
	
	argname = ""
	exit_after_init = False
	args= []

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

			elif args[0] == '-d':
				print 'deleting task'
			elif args[0] == '-i':
				print 'importing task'
			elif args[0] == '-l':
				print 'listing tasks'
			elif args[0] == '-x':
				print 'clearing tasks'
				clear_tasks( )

			else:
				raise Exception('Unknown command flag: ' + args[0])
		
		#  Finally, we need to load our task list
		tasklist = pyosutils.ls( program_data.task_directory, '.task')
		for task in tasklist:
			self.tasklist = Task.read(task)


	def usage( self ):
		
		print( "Usage:")


def create_task( GUI_MODE, options, screen = None ):
	
	global program_data

	#  Create a new task
	task = Task()
	
	# Create a list of indeces to keep track of cursor progress
	cursors=[0]

	#  Create a task based on the input options
	if GUI_MODE == TaskManager.NO_GUI:
		
		# We assume that they already provided the arguments
		task = Task( options.args )

		# compute a filename
		task.filename = program_data.task_directory + '/TASK_' + str(int(time.mktime(time.gmtime()))) + '.task'
		
		task.write();	
	
	#  Print a new GUI
	else:	
		while True:
		
			# Clear the console
			screen.clear()

			# Print the header 
			screen.addstr( 1, 1, 'Task Management Console')
			screen.addstr( 2, 1, '     Create Task')
			screen.addstr( 3, 1, '-----------------------')
			screen.addstr( 4, 1, 'Name: ' + task.name)
			screen.addstr( 5, 1, 'Groups: ' )

			#  Print the menu
			screen.refresh()

			# Grab user input
			selection = screen.getch()
	
			#  If the user wants to submit the task
			if selection == 13:
				return
			
			# If the user wants to change items

def clear_tasks( ):

	global program_data

	#  Find the task directory and get a list of contents
	files = pyosutils.ls( program_data.task_directory,  ['.task'])

	for file in files:
		pyosutils.rm( file )


def management_header( screen, options ):
	
	#  Grab the current window size
	size = screen.getmaxyx()

	# Print the top window
	screen.addstr( 1, 1, 'Task Management Console')
	screen.addstr( 2, 1, '-----------------------')
	screen.addstr( 3, 1, 'Current Open Tasks')
	
	# Print all open tasks


	# Print the footer
	screen.addstr( 8, 1, '-----------------------------')
	screen.addstr( 9, 1, '  Press any key to continue: ')
	screen.addstr( 10, 1, ' Options: Q-Exit Program, C-Create Task, D-Delete Task, V-View Tasks')



#   Management Console
def management_console( screen, options ):

	#  Set the screen delay
	screen.nodelay(True)
	screen.timeout(200)
	
	#  Start a loop to show the UI
	while True:

		#  Clear the console
		screen.clear()

		management_header( screen, options )



		# Grab user input
		screen.move(9,30)
		screen.refresh()
		selection = screen.getch()
		

		###################################
		#       Process User Input
		###################################
		#  Exit Console
		if selection == ord('Q') or selection == ord('q'):
			screen.addstr(9,30,'Exiting Program')
			screen.refresh()
			time.sleep(0.5)
			break
		elif selection == ord('C') or selection == ord('c'):
			create_task( TaskManager.GUI, options, screen )
	



#  Main Function
def main( ):
	
	#  Process command-line arguments
	options = Options( sys.argv )
	
	#  Read System Configuration
	global program_data
	program_data.read_config();

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


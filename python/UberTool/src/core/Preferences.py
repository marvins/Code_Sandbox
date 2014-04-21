#    File:    Preferences.py
#    Author:  Marvin Smith
#    Date:    4/18/2014
#
#    Purpose:  Store preferences for the GUI
#  

#  Python Libraries
import os


#  Preferences Class
class Preferences:
	
	# settings
	preferences = {}
	
	#  Constructor
	def __init__(self, args = [], configFilename = None, loadConfig = False ):
	
		#  Set the config filename if provided
		if not configFilename == None:
			self.configFilename = configFilename
		else:
			self.configFilename = os.environ['HOME'] + '/.ubertool/options.cfg'

		#  Initialize the container
		self.initializeSettings();

		#  Process command-line arguments


	#  Initialize Settings
	def initializeSettings(self):
		
		#  Add the module list
		self.preferences['AddOns.ModuleList']='module-list.txt'
		#  Add the button sizes
		self.preferences['core.MainWindowButtonWidth']='100';
		self.preferences['core.MainWindowButtonHeight']='100';

		#  Set default button icon sizes
		self.preferences['core.MainWindowButtonIconWidth']='70'
		self.preferences['core.MainWindowButtonIconHeight']='70'

		#  Set the default place for Main Window Icons
		self.preferences['core.IconHome']='src/icons';
		
	#  Open the config file
	def openConfigFile(self):
		
		pass

	#  Write the config file
	def saveConfigFile(self):
		
		pass

	#  Retrieve a setting
	def get(self, key):
		return self.preferences[key];

	#  Set a particular setting
	def set(self, key, value, DoNotClobber=True):
		
		#  Check if key is in dictionary
		if key in self.preferences and DoNotClobber == True:
			return False;

		#  If the key is not in the dictionary or if we don't care about clobbering, 
		#  then just write
		else:
			self.preferences[key]=value;

		return True;



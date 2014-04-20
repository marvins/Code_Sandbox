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

	#  Constructor
	def __init__(self, args = [], configFilename = None, loadConfig = False ):
	
		#  Set the config filename if provided
		if not configFilename == None:
			self.configFilename = configFilename
		else:
			self.configFilename = os.environ['HOME'] + '/.ubertool/options.cfg'

		#  Process command-line arguments


	#  Open the config file
	def openConfigFile(self):
		
		pass

	#  Write the config file
	def saveConfigFile(self):
		
		pass


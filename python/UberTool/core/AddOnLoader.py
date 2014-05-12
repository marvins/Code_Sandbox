#    File:    AddOnLoader.py
#    Author:  Marvin Smith
#    Date:    4/20/2014
#
#    Purpose:  Load all plugins in the add-on directory
#

#  Python Libraries
import imp, sys

#  Load Plugin Base
sys.path.insert(0,'add-ons')
import PluginBase

#   Add-On Loader
class AddOnLoader:
	
	plugins = []

	#  Constructor
	def __init__(self, configurationFile):
		
		#  Open the module configuration file
		modules = open(configurationFile,'r');
		
		#  Read Each List
		for line in modules:
			if not line.strip() == '':

				#  Split up the module name and the class name
				components = line.strip().split(':');

				py_mod = imp.load_source(components[1],'add-ons/' + components[0])

				class_inst = getattr(py_mod, components[1])
				self.plugins += [class_inst()]


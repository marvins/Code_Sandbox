#    File:    ConfigurationPane.py
#    Author:  Marvin Smith
#    Date:    4/18/2014
#
#    Purpose: Configure UberTool settings.
#

#   PyQt4
from PyQt4 import QtGui
from PyQt4.QtCore import *


#  Configuration Pane
class ConfigurationPane(QtGui.QDialog):
	
	
	#  Default Constructor
	def __init__(self, parent = None):
		
		#  Create parent
		QtGui.QDialog.__init__(self,parent);

		#  Set title
		self.setWindowTitle('Settings');



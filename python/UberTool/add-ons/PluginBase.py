#    File:    PluginBase.py
#    Author:  Marvin Smith
#    Date:    4/18/2014
#
#    Purpose:  Base Plugin 
#

#  PyQt4
from PyQt4 import QtGui
from PyQt4.QtCore import *

#  Plugin Class
class PluginBase(QtGui.QWidget):
	
	#  Constructor
	def __init__(self, parent = None):
		
		
		# Create parent
		QtGui.QWidget.__init__(self, parent);


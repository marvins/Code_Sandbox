#    File:    Calculator.py
#    Author:  Marvin Smith
#    Date:    4/18/2014
#    
#    Purpose:  Simple Calculator Add-On
#

#  PyQt4
from PyQt4 import QtGui
from PyQt4.QtCore import *

#  Plugin Base
import PluginBase

#  Calculator Plug-In
class Calculator(PluginBase.PluginBase):
	
	
	#  Constructor
	def __init__(self, parent = None):
		
		#  Create parent
		PluginBase.PluginBase.__init__(self, parent);

		#  Create Base Layout
		self.mainLayout = QtGui.QVBoxLayout();

		#  Create the main UI
		self.initUI()

		#  Set the main layout
		self.setLayout( self.mainLayout );

		 
	def initUI(self):
		
		#  set the title
		self.setWindowTitle('Calculator');


#    File:    CoordinateConverter.py
#    Author:  Marvin Smith
#    Date:    4/21/2014
#
#    Purpose:  Coordinate conversion utility
#

#  PyQt4
from PyQt4 import QtGui
from PyQt4.QtCore import *

#  Plugin Base
import PluginBase

def createCoordinatePanel( labelName ):

	#  Create a WIdget
	widget = QtGui.QWidget();

	#  Create the layout
	layout = QtGui.QVBoxLayout();

	#  Set the label
	mainLabel = QtGui.QLabel(labelName);
	layout.addWidget(mainLabel);

	#  Set the layout
	widget.setLayout(layout);

	return widget;


#  Calculator Plug-In
class CoordinateConverter(PluginBase.PluginBase):
	

	
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

	
	#  Open the dialog and show it
	def openDialog(self):
		
		self.show();
		 
	#  Initialize the User interface
	def initUI(self):
		
		#  set the title
		self.setWindowTitle('Coordinate Conversion');
		
		#  Create the main layout
		self.mainLayout = QtGui.QVBoxLayout();

		#  Create the from widget
		self.fromCoordinateWidget = createCoordinatePanel('From');
		self.mainLayout.addWidget(self.fromCoordinateWidget);

		#  Create the to widget
		self.toCoordinateWidget = createCoordinatePanel('To');
		self.mainLayout.addWidget(self.toCoordinateWidget);

		#  Set the layout
		self.setLayout(self.mainLayout);


	def getButtonIconSize(self):
		return QSize(70,50);

	def getButtonText(self):
		return 'Coordinate\nConverter'
	
	def getButtonIconPath(self):
		return 'coordinate.png';


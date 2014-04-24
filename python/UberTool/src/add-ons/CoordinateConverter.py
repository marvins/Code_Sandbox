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

#  Python Libraries
import sys

#  Add our conversion utilities
sys.path.insert(0,'src/add-ons/CoordinateConverter')
import CoordinateConverterUtils

#  This is the Coordinate Conversion Widget which stores our local info
class CoordinatePanel(QtGui.QWidget):
	
	def __init__(self, labelName, parent = None):
		
		#  Create parent
		QtGui.QWidget.__init__(self,parent);

		#  Create the layout
		self.layout = QtGui.QVBoxLayout();

		#  Set the label
		self.mainLabel = QtGui.QLabel(labelName);
		self.layout.addWidget(self.mainLabel);

		#  Create the coordinate type combo
		self.initCoordinateTypeWidget();
		
		#  Create the conversion stacked panel
		self.initCoordinateStackedWidget();

		#  Set the layout
		self.setLayout(self.layout);


	def initCoordinateTypeWidget(self):

		#  Create the Coordinate type
		self.coordinateTypeWidget = QtGui.QWidget();
		self.coordinateTypeLayout = QtGui.QHBoxLayout();
		
		#  Create the label
		self.coordinateTypeLabel = QtGui.QLabel('Coordinate Type:');
		self.coordinateTypeLayout.addWidget(self.coordinateTypeLabel);
	
		#  Create the combo box
		self.coordinateTypeCombo = QtGui.QComboBox();
		for ctype in CoordinateConverterUtils.CoordinateTypes:
			self.coordinateTypeCombo.addItem(ctype[0] + ' - ' + ctype[1]);
		self.coordinateTypeLayout.addWidget(self.coordinateTypeCombo);
		
		#  Connect the changed slot
		self.coordinateTypeCombo.currentIndexChanged.connect(self.changeCoordinateWindow);


		#  Set the layout
		self.coordinateTypeWidget.setLayout(self.coordinateTypeLayout);

		#  Add the widget
		self.layout.addWidget(self.coordinateTypeWidget);
	

	#  Create the stacked widget which will hold the conversion type window output.
	def initCoordinateStackedWidget(self):
		
		#  Create the stacked widget
		self.stackedWidget = QtGui.QStackedWidget();
		
		#  Create the coordinate dd widget
		self.stackedWidget.addWidget(CoordinateConverterUtils.GeodeticWindow());
		self.stackedWidget.addWidget(CoordinateConverterUtils.UTMWindow());


		#  Add widget
		self.layout.addWidget(self.stackedWidget);


	#  Change the coordinate window
	def changeCoordinateWindow(self):
		
		#  Get the current index
		cidx = self.coordinateTypeCombo.currentIndex();
		self.stackedWidget.setCurrentIndex(cidx);


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
		self.fromCoordinateWidget = CoordinatePanel('From');
		self.mainLayout.addWidget(self.fromCoordinateWidget);

		#  Create the to widget
		self.toCoordinateWidget = CoordinatePanel('To');
		self.mainLayout.addWidget(self.toCoordinateWidget);

		#  Set the layout
		self.setLayout(self.mainLayout);


	def getButtonIconSize(self):
		return QSize(70,50);

	def getButtonText(self):
		return 'Coordinate\nConverter'
	
	def getButtonIconPath(self):
		return 'coordinate.png';


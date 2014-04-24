#    File:    CoordinateConverterUtils.py
#    Author:  Marvin Smith
#    Date:    4/23/2014
#
#    Purpose:  Utilities for the CoordinateConverter GUI
#

#  PyQt4
from PyQt4 import QtGui
from PyQt4.QtCore import *


#   Define a set of conversion types we will support
CoordinateTypes=[('Geodetic','Geodetic DD/DM/DMS'),
                 ('UTM','Universal Transverse Mercator')
	  			];

#   Define a set of Datums we will support
Datums=[('WGS84','World Geodetic System of 1984'),
        ('WGS72','World Geodetic System of 1972'),
		('NAD83','North American Datum of 1983')
       ];


#  Geodetic Decimal Degrees Window
class GeodeticWindow(QtGui.QWidget):
	
	#  Constructor
	def __init__(self, parent=None):
		
		#  Create parent
		QtGui.QWidget.__init__(self,parent);

		#  Create the layout
		self.layout = QtGui.QGridLayout();

		#  Create the latitude dialog
		self.latitudeLabel = QtGui.QLabel('Latitude:');
		self.layout.addWidget(self.latitudeLabel, 0, 0);

		#  Create the latitude edit
		self.latitudeEdit = QtGui.QLineEdit();
		self.layout.addWidget(self.latitudeEdit, 0, 1, 1, 3);

		#  Create the longitude dialog
		self.longitudeLabel = QtGui.QLabel('Longitude:');
		self.layout.addWidget(self.longitudeLabel, 1, 0);

		#  Create the longitude edit
		self.longitudeEdit = QtGui.QLineEdit();
		self.layout.addWidget(self.longitudeEdit, 1, 1, 1, 3);
		
		#  Create the altitude label
		self.altitudeLabel = QtGui.QLabel('Altitude (m):');
		self.layout.addWidget(self.altitudeLabel, 2, 0 );
		
		#  Create the altitude edit
		self.altitudeEdit = QtGui.QLineEdit();
		self.layout.addWidget(self.altitudeEdit, 2, 1);

		#  Create the datum label
		self.datumLabel = QtGui.QLabel('Datum:');
		self.layout.addWidget(self.datumLabel, 2, 2 );

		#  Create the datum combo
		self.datumCombo = QtGui.QComboBox();
		for datum in Datums:
			self.datumCombo.addItem(datum[0]);
		self.layout.addWidget(self.datumCombo);

		#  Set the layout
		self.setLayout(self.layout);



#   UTM Window
class UTMWindow(QtGui.QWidget):
	
	#  Constructor
	def __init__(self, parent=None):

		#  Create parent
		QtGui.QWidget.__init__(self,parent);

		#  Create layout
		self.layout = QtGui.QGridLayout();

		#  Create easting label
		self.eastingLabel = QtGui.QLabel('Easting:');
		self.layout.addWidget(self.eastingLabel, 0, 0);

		#  Create easting edit
		self.eastingEdit = QtGui.QLineEdit();
		self.layout.addWidget(self.eastingEdit, 0, 1, 1, 3);
		
		#  Create northing label
		self.northingLabel = QtGui.QLabel('Northing:');
		self.layout.addWidget(self.northingLabel, 1, 0);

		#  Create northing edit
		self.northingEdit = QtGui.QLineEdit();
		self.layout.addWidget(self.northingEdit, 1, 1, 1, 3);
		
		#  Create the altitude label
		self.altitudeLabel = QtGui.QLabel('Altitude:');
		self.layout.addWidget(self.altitudeLabel, 2, 0);

		#  Create the altitude edit
		self.altitudeEdit = QtGui.QLineEdit();
		self.layout.addWidget(self.altitudeEdit, 2, 1);

		#  Create Zone label
		self.zoneLabel = QtGui.QLabel('Zone:');
		self.layout.addWidget(self.zoneLabel, 3, 0);

		#  Create zone combo
		self.zoneCombo = QtGui.QComboBox();
		for x in xrange(0,60):
			self.zoneCombo.addItem(str(x+1));
		self.layout.addWidget(self.zoneCombo, 3, 1);

		#  Create datum label
		self.datumLabel = QtGui.QLabel('Datum:');
		self.layout.addWidget(self.datumLabel, 3, 2);
		
		#  Create the datum combo
		self.datumCombo = QtGui.QComboBox();
		for datum in Datums:
			self.datumCombo.addItem(datum[0]);
		self.layout.addWidget(self.datumCombo, 3, 3);

		#  Set the layout
		self.setLayout(self.layout);





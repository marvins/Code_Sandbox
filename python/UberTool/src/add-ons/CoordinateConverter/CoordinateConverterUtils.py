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
	
	#  Signal for if anything is modified
	coordinatesModifiedSignal = pyqtSignal();

	#  Constructor
	def __init__(self, ReadOnly = False, parent=None):
		
		#  Create parent
		QtGui.QWidget.__init__(self,parent);
		
		#  Set the read only flag
		self.isReadOnly = ReadOnly

		#  Create the layout
		self.layout = QtGui.QGridLayout();

		#  Create the latitude dialog
		self.latitudeLabel = QtGui.QLabel('Latitude:');
		self.layout.addWidget(self.latitudeLabel, 0, 0);

		#  Create the latitude edit
		self.latitudeEdit = QtGui.QLineEdit();
		self.latitudeEdit.setText('0');
		self.latitudeEdit.setReadOnly( self.isReadOnly );
		self.latitudeEdit.textChanged.connect(self.updateCoordinateValues);
		self.layout.addWidget(self.latitudeEdit, 0, 1, 1, 3);

		#  Create the longitude dialog
		self.longitudeLabel = QtGui.QLabel('Longitude:');
		self.layout.addWidget(self.longitudeLabel, 1, 0);

		#  Create the longitude edit
		self.longitudeEdit = QtGui.QLineEdit();
		self.longitudeEdit.setText('0');
		self.longitudeEdit.setReadOnly(self.isReadOnly);
		self.longitudeEdit.textChanged.connect(self.updateCoordinateValues);
		self.layout.addWidget(self.longitudeEdit, 1, 1, 1, 3);
		
		#  Create the altitude label
		self.altitudeLabel = QtGui.QLabel('Altitude (m):');
		self.layout.addWidget(self.altitudeLabel, 2, 0 );
		
		#  Create the altitude edit
		self.altitudeEdit = QtGui.QLineEdit();
		self.altitudeEdit.setText('0');
		self.altitudeEdit.setReadOnly(self.isReadOnly);
		self.altitudeEdit.textChanged.connect(self.updateCoordinateValues);
		self.layout.addWidget(self.altitudeEdit, 2, 1);

		#  Create the datum label
		self.datumLabel = QtGui.QLabel('Datum:');
		self.layout.addWidget(self.datumLabel, 2, 2 );

		#  Create the datum combo
		self.datumCombo = QtGui.QComboBox();
		for datum in Datums:
			self.datumCombo.addItem(datum[0]);
		self.layout.addWidget(self.datumCombo);
		self.datumCombo.currentIndexChanged.connect(self.updateCoordinateValues);

		#  Set the layout
		self.setLayout(self.layout);
	
	#  Update the internal coordinate if anything is changed
	def updateCoordinateValues(self):
		
		#  Grab the values
		self.currentLatitude  = float(self.latitudeEdit.text());
		self.currentLongitude = float(self.longitudeEdit.text());
		self.currentAltitude  = float(self.altitudeEdit.text());

		#  Throw the signal
		self.coordinateModifiedSignal.emit();


	#  Return the latest coordinate
	def getCoordinate(self):
		
		#  Return the latest values
		return ['GEODETIC', True, self.currentLatitude, self.currentLongitude, self.currentAltitude, datum[self.datumCombo.currentIndex()]];



#   UTM Window
class UTMWindow(QtGui.QWidget):
	
	#  Signal for if anything is modified
	coordinatesModifiedSignal = pyqtSignal();
	
	#  Constructor
	def __init__(self, ReadOnly = False, parent=None):

		#  Create parent
		QtGui.QWidget.__init__(self,parent);

		#  Grab readonly flag
		self.isReadOnly = ReadOnly 

		#  Create layout
		self.layout = QtGui.QGridLayout();

		#  Create easting label
		self.eastingLabel = QtGui.QLabel('Easting:');
		self.layout.addWidget(self.eastingLabel, 0, 0);

		#  Create easting edit
		self.eastingEdit = QtGui.QLineEdit();
		self.eastingEdit.setReadOnly(self.isReadOnly);
		self.eastingEdit.textChanged.connect(self.updateCoordinateValues);
		self.layout.addWidget(self.eastingEdit, 0, 1, 1, 3);
		
		#  Create northing label
		self.northingLabel = QtGui.QLabel('Northing:');
		self.layout.addWidget(self.northingLabel, 1, 0);

		#  Create northing edit
		self.northingEdit = QtGui.QLineEdit();
		self.northingEdit.setReadOnly(self.isReadOnly);
		self.northingEdit.textChanged.connect(self.updateCoordinateValues);
		self.layout.addWidget(self.northingEdit, 1, 1, 1, 3);
		
		#  Create the altitude label
		self.altitudeLabel = QtGui.QLabel('Altitude:');
		self.layout.addWidget(self.altitudeLabel, 2, 0);

		#  Create the altitude edit
		self.altitudeEdit = QtGui.QLineEdit();
		self.altitudeEdit.setReadOnly(self.isReadOnly);
		self.altitudeEdit.textChanged.connect(self.updateCoordinateValues);
		self.layout.addWidget(self.altitudeEdit, 2, 1);

		#  Create Zone label
		self.zoneLabel = QtGui.QLabel('Zone:');
		self.layout.addWidget(self.zoneLabel, 3, 0);

		#  Create zone combo
		self.zoneCombo = QtGui.QComboBox();
		for x in xrange(0,60):
			self.zoneCombo.addItem(str(x+1));
		self.layout.addWidget(self.zoneCombo, 3, 1);
		self.zoneCombo.currentIndexChanged.connect(self.updateCoordinateValues);

		#  Create datum label
		self.datumLabel = QtGui.QLabel('Datum:');
		self.layout.addWidget(self.datumLabel, 3, 2);
		
		#  Create the datum combo
		self.datumCombo = QtGui.QComboBox();
		for datum in Datums:
			self.datumCombo.addItem(datum[0]);
		self.layout.addWidget(self.datumCombo, 3, 3);
		self.datumCombo.currentIndexChanged.connect(self.updateCoordinateValues);

		#  Set the layout
		self.setLayout(self.layout);

	
	#  Update the coordinate values
	def updateCoordinateValues(self):
		
		#  update the latitude, longitude, and altitude
		self.currentEasting  = float(self.eastingEdit.text());
		self.currentNorthing = float(self.northingEdit.text());
		self.currentAltitude = float(self.altitudeEdit.text());

		self.coordinateModifiedSignal.emit();

	
	#  grab the current coordinate
	def getCoordinate(self):
		
		return [];



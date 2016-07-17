#    File:    GeographicCoordinatePane.py
#    Author:  Marvin Smith
#    Date:    7/17/2016
#


#  Project Libraries
from plugins.CoordinateConverter.CoordinateBasePane import CoordinateBasePane
from plugins.CoordinateConverter.CoordinateConverterUtils import *

#  PyQt4
from PyQt4 import QtGui
from PyQt4.QtCore import *

#  Geodetic Decimal Degrees Window
class GeographicCoordinatePane(CoordinateBasePane):

    #  Signal for if anything is modified
    coordinateModifiedSignal = pyqtSignal()

    #  Constructor
    def __init__(self, ReadOnly = False, parent=None):

        #  Create parent
        CoordinateBasePane.__init__(self, ReadOnly, parent)

        #  Set the read only flag
        self.isReadOnly = ReadOnly

        #  Create the layout
        self.layout = QtGui.QGridLayout()

        #  Create the latitude dialog
        self.latitudeLabel = QtGui.QLabel('Latitude:')
        self.layout.addWidget(self.latitudeLabel, 0, 0)

        #  Create the latitude edit
        self.latitudeEdit = QtGui.QLineEdit()
        self.latitudeEdit.setText('0')
        self.latitudeEdit.setReadOnly( self.isReadOnly )
        self.latitudeEdit.textChanged.connect(self.updateCoordinateValues)
        self.layout.addWidget(self.latitudeEdit, 0, 1, 1, 3)

        #  Create the longitude dialog
        self.longitudeLabel = QtGui.QLabel('Longitude:')
        self.layout.addWidget(self.longitudeLabel, 1, 0)

        #  Create the longitude edit
        self.longitudeEdit = QtGui.QLineEdit()
        self.longitudeEdit.setText('0')
        self.longitudeEdit.setReadOnly(self.isReadOnly)
        self.longitudeEdit.textChanged.connect(self.updateCoordinateValues)
        self.layout.addWidget(self.longitudeEdit, 1, 1, 1, 3)

        #  Create the altitude label
        self.altitudeLabel = QtGui.QLabel('Altitude (m):')
        self.layout.addWidget(self.altitudeLabel, 2, 0 )

        #  Create the altitude edit
        self.altitudeEdit = QtGui.QLineEdit()
        self.altitudeEdit.setText('0')
        self.altitudeEdit.setReadOnly(self.isReadOnly)
        self.altitudeEdit.textChanged.connect(self.updateCoordinateValues)
        self.layout.addWidget(self.altitudeEdit, 2, 1)

        #  Create the datum label
        self.datumLabel = QtGui.QLabel('Datum:')
        self.layout.addWidget(self.datumLabel, 2, 2 )

        #  Create the datum combo
        self.datumCombo = QtGui.QComboBox()
        for datum in Datums:
            self.datumCombo.addItem(datum[0])
        self.layout.addWidget(self.datumCombo)
        self.datumCombo.currentIndexChanged.connect(self.updateCoordinateValues)

        #  Set the layout
        self.setLayout(self.layout)

    #  Update the internal coordinate if anything is changed
    def updateCoordinateValues(self):

        #  Grab the values
        self.currentLatitude  = float(self.latitudeEdit.text())
        self.currentLongitude = float(self.longitudeEdit.text())
        self.currentAltitude  = float(self.altitudeEdit.text())

        #  Throw the signal
        self.coordinateModifiedSignal.emit()


    #  Return the latest coordinate
    def getCoordinate(self):

        #  Return the latest values
        return ['GEODETIC',
                True,
                self.currentLatitude,
                self.currentLongitude,
                self.currentAltitude,
                datum[self.datumCombo.currentIndex()]]


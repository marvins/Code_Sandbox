#   File:    UTMCoordinatePane.py
#   Author:  Marvin Smith
#   Date:    7/17/2016
#


#  Project Libraries
from plugins.CoordinateConverter.CoordinateBasePane import CoordinateBasePane
from plugins.CoordinateConverter.CoordinateConverterUtils import *


#  PyQt4
from PyQt4 import QtGui
from PyQt4.QtCore import *

class UTMCoordinatePane(CoordinateBasePane):

    #  Signal for if anything is modified
    coordinatesModifiedSignal = pyqtSignal()

    #  Constructor
    def __init__(self, ReadOnly = False, parent=None):

        #  Create parent
        CoordinateBasePane.__init__(self, ReadOnly, parent)

        #  Grab readonly flag
        self.isReadOnly = ReadOnly

        #  Create layout
        self.layout = QtGui.QGridLayout()

        #  Create easting label
        self.eastingLabel = QtGui.QLabel('Easting:')
        self.layout.addWidget(self.eastingLabel, 0, 0)

        #  Create easting edit
        self.eastingEdit = QtGui.QLineEdit()
        self.eastingEdit.setReadOnly(self.isReadOnly)
        self.eastingEdit.textChanged.connect(self.updateCoordinateValues)
        self.layout.addWidget(self.eastingEdit, 0, 1, 1, 3)

        #  Create northing label
        self.northingLabel = QtGui.QLabel('Northing:')
        self.layout.addWidget(self.northingLabel, 1, 0)

        #  Create northing edit
        self.northingEdit = QtGui.QLineEdit()
        self.northingEdit.setReadOnly(self.isReadOnly)
        self.northingEdit.textChanged.connect(self.updateCoordinateValues)
        self.layout.addWidget(self.northingEdit, 1, 1, 1, 3)

        #  Create the altitude label
        self.altitudeLabel = QtGui.QLabel('Altitude:')
        self.layout.addWidget(self.altitudeLabel, 2, 0)

        #  Create the altitude edit
        self.altitudeEdit = QtGui.QLineEdit()
        self.altitudeEdit.setReadOnly(self.isReadOnly)
        self.altitudeEdit.textChanged.connect(self.updateCoordinateValues)
        self.layout.addWidget(self.altitudeEdit, 2, 1)

        #  Create Zone label
        self.zoneLabel = QtGui.QLabel('Zone:')
        self.layout.addWidget(self.zoneLabel, 3, 0)

        #  Create zone combo
        self.zoneCombo = QtGui.QComboBox()
        for x in xrange(0,60):
            self.zoneCombo.addItem(str(x+1))
        self.layout.addWidget(self.zoneCombo, 3, 1)
        self.zoneCombo.currentIndexChanged.connect(self.updateCoordinateValues)

        #  Create datum label
        self.datumLabel = QtGui.QLabel('Datum:')
        self.layout.addWidget(self.datumLabel, 3, 2)

        #  Create the datum combo
        self.datumCombo = QtGui.QComboBox()
        for datum in Datums:
            self.datumCombo.addItem(datum[0])
        self.layout.addWidget(self.datumCombo, 3, 3)
        self.datumCombo.currentIndexChanged.connect(self.updateCoordinateValues)

        #  Set the layout
        self.setLayout(self.layout)


    #  Update the coordinate values
    def updateCoordinateValues(self):

        try:
            #  update the latitude, longitude, and altitude
            if len(str(self.eastingEdit.text())) > 0:
                self.currentEasting  = float(self.eastingEdit.text())
            if len(str(self.northingEdit.text())) > 0:
                self.currentNorthing = float(self.northingEdit.text())
            if len(str(self.altitudeEdit.text())) > 0:
                self.currentAltitude = float(self.altitudeEdit.text())
        except Exception:
            pass

        self.coordinateModifiedSignal.emit()


    #  grab the current coordinate
    def getCoordinate(self):
        return []

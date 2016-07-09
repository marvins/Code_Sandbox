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
from plugins.PluginBase import PluginBase

#  Python Libraries
import sys

#  Add our conversion utilities
from plugins.CoordinateConverter import CoordinateConverterUtils


#  This is the Coordinate Conversion Widget which stores our local info
class CoordinatePanel(QtGui.QWidget):


    def __init__(self, labelName, ReadOnly = False, parent = None):

        #  Create parent
        QtGui.QWidget.__init__(self,parent)

        #  Set the read/modify flag
        self.isReadOnly = ReadOnly

        #  Create the layout
        self.layout = QtGui.QVBoxLayout()

        #  Set the label
        self.mainLabel = QtGui.QLabel(labelName)
        self.layout.addWidget(self.mainLabel)

        #  Create the coordinate type combo
        self.initCoordinateTypeWidget()

        #  Create the conversion stacked panel
        self.initCoordinateStackedWidget()

        #  Set the layout
        self.setLayout(self.layout)


    def initCoordinateTypeWidget(self):

        #  Create the Coordinate type
        self.coordinateTypeWidget = QtGui.QWidget()
        self.coordinateTypeLayout = QtGui.QHBoxLayout()

        #  Create the label
        self.coordinateTypeLabel = QtGui.QLabel('Coordinate Type:')
        self.coordinateTypeLayout.addWidget(self.coordinateTypeLabel)

        #  Create the combo box
        self.coordinateTypeCombo = QtGui.QComboBox()
        for ctype in CoordinateConverterUtils.CoordinateTypes:
            self.coordinateTypeCombo.addItem(ctype[0] + ' - ' + ctype[1])
        self.coordinateTypeLayout.addWidget(self.coordinateTypeCombo)

        #  Connect the changed slot
        self.coordinateTypeCombo.currentIndexChanged.connect(self.changeCoordinateWindow)


        #  Set the layout
        self.coordinateTypeWidget.setLayout(self.coordinateTypeLayout)

        #  Add the widget
        self.layout.addWidget(self.coordinateTypeWidget)


    #  Create the stacked widget which will hold the conversion type window output.
    def initCoordinateStackedWidget(self):

        #  Create the stacked widget
        self.stackedWidget = QtGui.QStackedWidget()

        #  Create the coordinate dd widget
        self.stackedWidget.addWidget(CoordinateConverterUtils.GeodeticWindow(ReadOnly = self.isReadOnly ))
        self.stackedWidget.addWidget(CoordinateConverterUtils.UTMWindow( ReadOnly = self.isReadOnly ))


        #  Add widget
        self.layout.addWidget(self.stackedWidget)


    #  Change the coordinate window
    def changeCoordinateWindow(self):

        #  Get the current index
        cidx = self.coordinateTypeCombo.currentIndex()
        self.stackedWidget.setCurrentIndex(cidx)


#  Calculator Plug-In
class CoordinateConverter(PluginBase):

    #  Constructor
    def __init__(self, preferences, parent = None):

        #  Create parent
        PluginBase.__init__(self, preferences, parent)

        #  Create Base Layout
        self.mainLayout = QtGui.QVBoxLayout()

        #  Create the main UI
        self.Initialize_UI()

        #  Set the main layout
        self.setLayout( self.mainLayout )


    #  Open the dialog and show it
    def Open_Dialog(self):

        self.show()

    #  Initialize the User interface
    def Initialize_UI(self):

        #  set the title
        self.setWindowTitle('Coordinate Conversion')

        #  Create the main layout
        self.mainLayout = QtGui.QVBoxLayout()

        #  Create the from widget
        self.fromCoordinateWidget = CoordinatePanel('From', ReadOnly = False)
        self.mainLayout.addWidget(self.fromCoordinateWidget)

        #  Create the to widget
        self.toCoordinateWidget = CoordinatePanel('To', ReadOnly = True)
        self.mainLayout.addWidget(self.toCoordinateWidget)

        #  Create the toolbar

        #  Set the layout
        self.setLayout(self.mainLayout)


    #  Get the icon size for the buttons
    def Get_Button_Icon_Size(self):
        return QSize(70,50)

    #  Get the text to put on the button
    def Get_Button_Text(self):
        return 'Coordinate\nConverter'

    #  Get the file path to the button icon
    def Get_Button_Icon_Path(self):
        return 'coordinate.png'


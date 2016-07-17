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
from plugins.CoordinateConverter.GeographicCoordinatePane import GeographicCoordinatePane
from plugins.CoordinateConverter.UTMCoordinatePane import UTMCoordinatePane


#  This is the Coordinate Conversion Widget which stores our local info
class CoordinatePanel(QtGui.QGroupBox):


    def __init__(self, labelName, ReadOnly = False, parent = None):

        #  Create parent
        QtGui.QGroupBox.__init__(self, labelName, parent)

        #  Set the read/modify flag
        self.isReadOnly = ReadOnly

        #  Create the layout
        self.layout = QtGui.QVBoxLayout()

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
        self.stackedWidget.addWidget(GeographicCoordinatePane( ReadOnly = self.isReadOnly ))
        self.stackedWidget.addWidget(UTMCoordinatePane( ReadOnly = self.isReadOnly ))


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
        self.Build_Toolbar()

        #  Set the layout
        self.setLayout(self.mainLayout)

    def Build_Toolbar(self):

        #  Create Toolbar Widget
        self.m_toolbar_widget = QtGui.QGroupBox('Toolbar', self)

        #  Create Layout
        self.m_toolbar_layout = QtGui.QHBoxLayout()
        self.m_toolbar_layout.setAlignment(Qt.AlignLeft)

        #  Create Close Button
        icon_path=self.preferences.Query('CORE','ICON_HOME')+'/close.png'
        self.m_toolbar_close_button = self.Create_Button(action=self.close,
                                                         icon_path=icon_path,
                                                         tooltip='Close Panel.')
        self.m_toolbar_layout.addWidget(self.m_toolbar_close_button)


        #  Set layout
        self.m_toolbar_widget.setLayout(self.m_toolbar_layout)

        #  Add to main
        self.mainLayout.addWidget(self.m_toolbar_widget)

    #  Get the Module Name
    def Get_Plugin_Name(self):
        return 'CoordinateConverter'

    #  Get the icon size for the buttons
    def Get_Button_Icon_Size(self):
        return QSize(70,50)

    #  Get the text to put on the button
    def Get_Button_Text(self):
        return 'Coordinate\nConverter'

    #  Get the file path to the button icon
    def Get_Button_Icon_Path(self):
        return 'coordinate.png'

    @staticmethod
    def Get_Default_Configuration_Options():
        return {'coordinate_converter.ToolbarButtonWidth':      ['40','Width of Toolbar Buttons'],
                'coordinate_converter.ToolbarButtonHeight':     ['40','Height of Toolbar Buttons.'],
                'coordinate_converter.ToolbarButtonIconWidth':  ['30','Width of Toolbar Button Icons'],
                'coordinate_converter.ToolbarButtonIconHeight': ['30','Height of Toolbar Button Icons']}



    def Create_Button( self, name = None,
                             parent = None,
                             action=None,
                             icon_path = None,
                             icon_size = None,
                             tooltip = None):

        #  Create ToolButton
        button = QtGui.QToolButton(parent)

        #  Set attributes
        if not name is None:
            button.setText(name)
            button.setToolButtonStyle(Qt.ToolButtonTextUnderIcon)

        if not action is None:
            button.clicked.connect(action)

        button.setFixedWidth(int(self.preferences.QueryPlugin('coordinate_converter.ToolbarButtonWidth',100)))
        button.setFixedHeight(int(self.preferences.QueryPlugin('coordinate_converter.ToolbarButtonHeight',100)))

        if not icon_path is None:
            button.setIcon(QtGui.QIcon(icon_path))

        if icon_size is None:
            button.setIconSize(QSize(int(self.preferences.QueryPlugin('coordinate_converter.ToolbarButtonIconWidth')),
                                     int(self.preferences.QueryPlugin('coordinate_converter.ToolbarButtonIconHeight'))))
        else:
            button.setIconSize(icon_size)


        if not tooltip is None:
            self.setToolTip(tooltip)

        return button
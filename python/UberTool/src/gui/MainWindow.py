#    File:   MainWindow.py
#    Author: Marvin Smith
#    Date:   4/18/2014
#
#    Purpose:  Main GUI Window
#

#  PyQt4
from PyQt4 import QtGui
from PyQt4.QtCore import *

# Python Libraries
import sys

#  Add Core Modules
from core import AddOnLoader, Preferences


#  Add GUI Modules
from ConfigurationPane import *

#  Main GUI Window
class MainWindow(QtGui.QMainWindow):

    #  Constructor
    def __init__(self, preferences = None):

        #  Create parent
        QtGui.QMainWindow.__init__(self)

        #  Set preferences 
        self.m_preferences = preferences

        #  Create the configuration pane
        self.m_configuration_pane = ConfigurationPane()

        #  Load Plugins
        self.Load_Plugins()

        #  Initialize the GUI
        self.Initialize_UI()

        #  Configure Window
        self.setWindowState( self.windowState() & ~Qt.WindowMinimized | Qt.WindowActive )

        #  Show the GUI
        self.show()

        self.activateWindow()


    #  Initialize the User Interface
    def Initialize_UI(self):

        #  Set the window title
        self.setWindowTitle(self.m_preferences.Query('GUI','MAIN_WINDOW_TITLE','UberTool App'))

        #  Create the scroll area
        self.m_mainScrollArea = QtGui.QScrollArea(self)

        #  Create the main widget
        self.m_mainWidget = QtGui.QWidget(self.m_mainScrollArea)

        #  Create the main layout
        self.m_mainLayout = QtGui.QGridLayout()
        self.m_mainLayout.setSpacing(0)
        self.m_mainLayout.setContentsMargins(0,0,0,0)

        #  Initialize the toolbar
        self.Initialize_Toolbar()

        #  Set the main widget
        self.m_mainWidget.setLayout(self.m_mainLayout)

        #  Set the scroll area
        self.m_mainScrollArea.setWidget(self.m_mainWidget)

        #  Set the central widget
        self.setCentralWidget(self.m_mainScrollArea)


    #  Initialize the toolbar
    def Initialize_Toolbar(self):

        crow = 0
        ccol = 0

        #  Add each button to the toolbar
        for x in xrange(0, len(self.pluginButtons)):
            self.m_mainLayout.addWidget( self.pluginButtons[x], crow, ccol)
            ccol += 1
            if ccol >= int(self.m_preferences.Query('GUI','MainWindowButtonsPerRow')):
                ccol = 0
                crow += 1

        if not ccol == 0:
            ccol = 0
            crow += 1

        #  Create the config button
        self.m_config_button = self.Create_Main_Button( 'Configure',
                                                        self.m_mainWidget,
                                                        action=self.Open_Configuration_Pane,
                                                        icon_path=self.m_preferences.Query('CORE','ICON_HOME')+'/gear.png')
        self.m_mainLayout.addWidget(self.m_config_button, crow, ccol)

        #  Create the quit button
        ccol += 1
        self.m_quit_button = self.Create_Main_Button('Quit',
                                                     self.m_mainWidget,
                                                     action=self.close,
                                                     icon_path=self.m_preferences.Query('CORE','ICON_HOME')+'/close.png')
        self.m_mainLayout.addWidget( self.m_quit_button,
                                     crow,
                                     ccol)


    #  Load each plugin
    def Load_Plugins(self):

        # open the add-on loader
        addOnLoader = AddOnLoader.AddOnLoader( self.m_preferences )

        # Get a list of plugins
        self.plugins = addOnLoader.plugins
        self.pluginButtons = []

        #  Add each plugin to the plugin-list
        for x in xrange( 0, len(self.plugins)):

            #  Create the button
            icon_path = self.m_preferences.Query('CORE','ICON_HOME') + '/' + self.plugins[x].Get_Button_Icon_Path()
            pluginButton = self.Create_Main_Button( name=self.plugins[x].Get_Button_Text(),
                                                    parent=None,
                                                    action=self.plugins[x].Open_Dialog,
                                                    icon_path=icon_path,
                                                    icon_size=self.plugins[x].Get_Button_Icon_Size())
            self.pluginButtons.append(pluginButton)


    #  Open the configuration pane
    def Open_Configuration_Pane(self):

        #  Open the configuration widget
        self.m_configuration_pane.show()


    def Create_Main_Button( self, name, parent, action=None, icon_path = None, icon_size = None ):

        #  Create ToolButton
        button = QtGui.QToolButton(parent)

        #  Set attributes
        button.setText(name)

        if not action is None:
            button.clicked.connect(action)

        button.setFixedWidth(int(self.m_preferences.Query('GUI','MainWindowButtonWidth',100)))
        button.setFixedHeight(int(self.m_preferences.Query('GUI','MainWindowButtonHeight',100)))

        if not icon_path is None:
            button.setIcon(QtGui.QIcon(icon_path))

        if icon_size is None:
            button.setIconSize(QSize(int(self.m_preferences.Query('GUI','MainWindowButtonIconWidth')),
                                     int(self.m_preferences.Query('GUI','MainWindowButtonIconHeight'))))
        else:
            button.setIconSize(icon_size)

        button.setToolButtonStyle(Qt.ToolButtonTextUnderIcon)

        return button

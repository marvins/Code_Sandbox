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
        super( MainWindow, self).__init__()

        #  Set preferences 
        self.preferences = preferences

        #  Create the configuration pane
        self.configurationPane = ConfigurationPane()

        #  Load Plugins
        self.loadPlugins()

        #  Initialize the GUI
        self.initUI()

        #  Show the GUI
        self.show()


    #  Initialize the User Interface
    def initUI(self):

        #  Set the window title
        self.setWindowTitle('UberTool')

        #  Create the scroll area
        self.mainScrollArea = QtGui.QScrollArea(self)

        #  Create the main widget
        self.mainWidget = QtGui.QWidget(self.mainScrollArea)

        #  Create the main layout
        self.mainLayout = QtGui.QGridLayout()
        self.mainLayout.setSpacing(0)
        self.mainLayout.setContentsMargins(0,0,0,0)

        #  Initialize the toolbar
        self.initToolbar()

        #  Set the main widget
        self.mainWidget.setLayout(self.mainLayout)

        #  Set the scroll area
        self.mainScrollArea.setWidget(self.mainWidget)

        #  Set the central widget
        self.setCentralWidget(self.mainScrollArea)


    #  Initialize the toolbar
    def initToolbar(self):

        crow = 0
        ccol = 0

        #  Add each button to the toolbar
        for x in xrange(0, len(self.pluginButtons)):
            self.mainLayout.addWidget( self.pluginButtons[x], crow, ccol)
            ccol += 1
            if ccol >= int(self.preferences.get('core.ButtonsPerRow')):
                ccol = 0
                crow += 1

        if not ccol == 0:
            ccol = 0
            crow += 1

        #  Create the config button
        self.configButton = QtGui.QToolButton(self.mainWidget)
        self.configButton.setText('Configure')
        self.configButton.setFixedWidth(int(self.preferences.get('core.MainWindowButtonWidth')))
        self.configButton.setFixedHeight(int(self.preferences.get('core.MainWindowButtonHeight')))
        self.configButton.setIcon(QtGui.QIcon(self.preferences.get('core.IconHome')+'/gear.png'))
        self.configButton.setIconSize(QSize(int(self.preferences.get('core.MainWindowButtonIconWidth')), int(self.preferences.get('core.MainWindowButtonIconHeight'))))
        self.configButton.setToolButtonStyle(Qt.ToolButtonTextUnderIcon)
        self.configButton.clicked.connect(self.openConfigurePane)
        self.mainLayout.addWidget(self.configButton, crow, ccol)

        #  Create the quit button
        ccol += 1
        self.quitButton = QtGui.QToolButton(self.mainWidget)
        self.quitButton.setText('Quit')
        self.quitButton.setFixedWidth(int(self.preferences.get('core.MainWindowButtonWidth')))
        self.quitButton.setFixedHeight(int(self.preferences.get('core.MainWindowButtonHeight')))
        self.quitButton.setIcon(QtGui.QIcon(self.preferences.get('core.IconHome')+'/close.png'))
        self.quitButton.setIconSize(QSize(int(self.preferences.get('core.MainWindowButtonIconWidth')), int(self.preferences.get('core.MainWindowButtonIconHeight'))))
        self.quitButton.setToolButtonStyle(Qt.ToolButtonTextUnderIcon)
        self.quitButton.clicked.connect(self.close)
        self.mainLayout.addWidget(self.quitButton, crow, ccol)

    #  Load each plugin
    def loadPlugins(self):

        # open the add-on loader
        addOnLoader = AddOnLoader.AddOnLoader( self.preferences.get('AddOns.ModuleList'))

        # Get a list of plugins
        self.plugins = addOnLoader.plugins
        self.pluginButtons = []

        #  Add each plugin to the plugin-list
        for x in xrange( 0, len(self.plugins)):

            #  Create the button
            pluginButton = QtGui.QToolButton();
            pluginButton.setText(self.plugins[x].getButtonText());
            pluginButton.setIcon(QtGui.QIcon(self.preferences.get('core.IconHome') + '/' + self.plugins[x].getButtonIconPath()));
            pluginButton.setFixedWidth(int(self.preferences.get('core.MainWindowButtonWidth')));
            pluginButton.setFixedHeight(int(self.preferences.get('core.MainWindowButtonHeight')));
            pluginButton.setIconSize(self.plugins[x].getButtonIconSize());
            pluginButton.setToolButtonStyle(Qt.ToolButtonTextUnderIcon);
            pluginButton.clicked.connect(self.plugins[x].openDialog);
            self.pluginButtons.append(pluginButton);


    #  Open the configuration pane
    def openConfigurePane(self):

        #  Open the configuration widget
        self.configurationPane.show()


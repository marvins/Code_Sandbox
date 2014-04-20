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
sys.path.insert(0,'src/core')
import Preferences

#  Add GUI Modules
from ConfigurationPane import *

#  Main GUI Window
class MainWindow(QtGui.QMainWindow):
	
	#  List of plugins
	plugins = []

	#  Preferences
	preferences = Preferences.Preferences();

	#  Constructor
	def __init__(self, preferences = None):

		#  Create parent
		super( MainWindow, self).__init__()

		#  Set preferences 
		if not preferences == None:
			self.preferences = preferences;
		
		#  Create the configuration pane
		self.configurationPane = ConfigurationPane();

		#  Load Plugins
		self.loadPlugins();

		#  Initialize the GUI
		self.initUI();

		#  Show the GUI
		self.show();


	#  Initialize the User Interface
	def initUI(self):
		
		#  Set the window title
		self.setWindowTitle('UberTool');

		#  Create the scroll area
		self.mainScrollArea = QtGui.QScrollArea(self);

		#  Create the main widget
		self.mainWidget = QtGui.QWidget(self.mainScrollArea);

		#  Create the main layout
		self.mainLayout = QtGui.QGridLayout();

		#  Initialize the toolbar
		self.initToolbar();
		
		#  Set the main widget
		self.mainWidget.setLayout(self.mainLayout);

		#  Set the scroll area
		self.mainScrollArea.setWidget(self.mainWidget);

		#  Set the central widget
		self.setCentralWidget(self.mainScrollArea);


	#  Initialize the toolbar
	def initToolbar(self):
	
		#  Create the config button
		self.configButton = QtGui.QToolButton(self.mainWidget);
		self.configButton.setText('Configure');
		self.configButton.setFixedWidth(int(self.preferences.get('core.MainWindowButtonWidth')));
		self.configButton.setFixedHeight(int(self.preferences.get('core.MainWindowButtonHeight')));
		self.configButton.setIcon(QtGui.QIcon(self.preferences.get('core.IconHome')+'/gear.png'));
		self.configButton.setIconSize(QSize(int(self.preferences.get('core.MainWindowButtonIconWidth')), int(self.preferences.get('core.MainWindowButtonIconHeight'))));
		self.configButton.setToolButtonStyle(Qt.ToolButtonTextUnderIcon);
		self.configButton.clicked.connect(self.openConfigurePane)
		self.mainLayout.addWidget(self.configButton, 0, 0);
		
		#  Create the quit button
		self.quitButton = QtGui.QToolButton(self.mainWidget);
		self.quitButton.setText('Quit');
		self.quitButton.setFixedWidth(int(self.preferences.get('core.MainWindowButtonWidth')));
		self.quitButton.setFixedHeight(int(self.preferences.get('core.MainWindowButtonHeight')));
		self.quitButton.setIcon(QtGui.QIcon(self.preferences.get('core.IconHome')+'/close.png'));
		self.quitButton.setIconSize(QSize(int(self.preferences.get('core.MainWindowButtonIconWidth')), int(self.preferences.get('core.MainWindowButtonIconHeight'))));
		self.quitButton.setToolButtonStyle(Qt.ToolButtonTextUnderIcon);
		self.quitButton.clicked.connect(self.close)
		self.mainLayout.addWidget(self.quitButton, 0, 1);

	#  Load each plugin
	def loadPlugins(self):
		
		pass


	#  Open the configuration pane
	def openConfigurePane(self):
	
		#  Open the configuration widget
		self.configurationPane.show();


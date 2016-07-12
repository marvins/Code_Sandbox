#    File:    ConfigurationPane.py
#    Author:  Marvin Smith
#    Date:    4/18/2014
#
#    Purpose: Configure UberTool settings.
#

#   PyQt4
from PyQt4 import QtGui
from PyQt4.QtCore import *


from gui.ConfigurationFilePane import ConfigurationFilePane
from gui.LogViewerPane import LogViewerPane

#  Configuration Pane
class ConfigurationPane(QtGui.QDialog):


    #  Default Constructor
    def __init__(self, options, parent = None):

        #  Create parent
        QtGui.QDialog.__init__(self,parent)

        #  Set Preferences
        self.m_options = options

        #  Set title
        self.setWindowTitle('Settings')

        #  Set Defaults
        self.m_tree_labels = ['Module','Name','Value','Description']

        # Initialize the GUI
        self.Initialize_UI()

        #  Connect Signals and Slots
        self.Connect_Widgets()

    def Initialize_UI(self):

        #  Create the main widget
        self.m_main_layout = QtGui.QVBoxLayout()

        #  Create Stacked Widget
        self.m_tab_widget = QtGui.QTabWidget()

        #  Create Configuration Panel
        self.Build_Configuration_Panel()

        #  Create Log Panel
        self.Build_Log_Panel()

        #  Add to layout
        self.m_main_layout.addWidget(self.m_tab_widget)

        #  Set the layout
        self.setLayout(self.m_main_layout)

    def Build_Configuration_Panel(self):

        #  Create Config File Pane
        self.m_config_pane = ConfigurationFilePane(self.m_options, self)
        self.m_tab_widget.addTab(self.m_config_pane, 'Config Editor')

    def Build_Log_Panel(self):

        #  Create pane
        self.m_log_pane = LogViewerPane(self.m_options, self)
        self.m_tab_widget.addTab(self.m_log_pane, 'Log Viewer')

    def Connect_Widgets(self):

        #  Listen for close
        self.connect( self.m_log_pane,    SIGNAL('close_panel()'), self.close)
        self.connect( self.m_config_pane, SIGNAL('close_panel()'), self.close)

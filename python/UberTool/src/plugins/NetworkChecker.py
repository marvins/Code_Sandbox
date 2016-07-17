#    File:    NetworkChecker.py
#    Author:  Marvin Smith
#    Date:    7/12/2016
#
#    Purpose:  Network Scanning Utility
#

#  PyQt4
from PyQt4 import QtGui
from PyQt4.QtCore import *

#  Plugin Base
from plugins.PluginBase import PluginBase

#  Python Libraries
import sys

#  Add our conversion utilities
from plugins.NetworkChecker import NetworkCheckerUtils


class NetworkChecker(PluginBase):


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
        self.setWindowTitle('Network Analysis')

        #  Create the main layout
        self.mainLayout = QtGui.QVBoxLayout()


        #  Set the layout
        self.setLayout(self.mainLayout)

    #  Get the Module Name
    def Get_Plugin_Name(self):
        return 'NetworkChecker'

    #  Get the icon size for the buttons
    def Get_Button_Icon_Size(self):
        return QSize(70,50)

    #  Get the text to put on the button
    def Get_Button_Text(self):
        return 'Network\nAnalysis'

    #  Get the file path to the button icon
    def Get_Button_Icon_Path(self):
        return 'network.png'

    @staticmethod
    def Get_Default_Configuration_Options():
        return {'network_checker.ToolbarButtonIconWidth':  ['30','Icon Width of Toolbar Buttons'],
                'network_checker.ToolbarButtonIconHeight': ['30','Icon Height of Toolbar Buttons']}

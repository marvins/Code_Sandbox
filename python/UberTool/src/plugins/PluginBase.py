#    File:    PluginBase.py
#    Author:  Marvin Smith
#    Date:    4/18/2014
#
#    Purpose:  Base Plugin 
#

#  PyQt4
from PyQt4 import QtGui
from PyQt4.QtCore import *

#  Plugin Class
class PluginBase(QtGui.QWidget):

    #  Constructor
    def __init__(self, preferences = None, parent = None):

        # Create parent
        QtGui.QWidget.__init__(self, parent)

        #  Set Preferences
        self.preferences = preferences

    def Get_Button_Text(self):
        return 'None-Provided.'

    def Get_Button_Icon_Size(self):
        return QtGui.QSize(100,100)


    @staticmethod
    def Get_Default_Configuration_Options():
        return {}

#    File:    AddConfigOptionPane.py
#    Author:  Marvin Smith
#    Date:    7/17/2016
#
#    Purpose: Add option to GUI.
#


#   PyQt4
from PyQt4 import QtGui
from PyQt4.QtCore import *


class AddConfigOptionPane(QtGui.QDialog):

    def __init__(self, parent = None):

        #  Create parent
        QtGui.QDialog.__init__(self, parent)



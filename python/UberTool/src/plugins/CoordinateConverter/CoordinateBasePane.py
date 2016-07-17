#    File:    CoordinateBasePane.py
#    Author:  Marvin Smith
#    Date:    7/17/2016
#


#  PyQt4
from PyQt4 import QtGui
from PyQt4.QtCore import *


class CoordinateBasePane(QtGui.QWidget):

    def __init__(self, ReadOnly = False, parent = None):

        #  Build Parent
        QtGui.QWidget.__init__(self, parent)

        #  Set the Read-Only Flag
        self.m_read_only = ReadOnly
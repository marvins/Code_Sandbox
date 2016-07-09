#!/usr/bin/env python
#
#    File:    UberTool.py
#    Author:  Marvin Smith
#    Date:    4/18/2014
#

#  System Libraries
import sys, logging, os

#  PyQt4
from PyQt4 import QtGui

#  UberTool Base Path
sys.path.insert(0, os.path.dirname(__file__))

#  Core Files
from core.Preferences import Preferences

#  Main Window
from gui.MainWindow import MainWindow


# -----------------------------#
# -       Main Function       -#
# -----------------------------#
def main():

    #  Load the configuration file
    preferences = Preferences()

    # Create application
    app = QtGui.QApplication(sys.argv)

    #  Load the Main Widget
    mainWindow = MainWindow(preferences)

    #  Start the application
    sys.exit(app.exec_())


# -----------------------------#
# -     Start of Program      -#
# -----------------------------#
if __name__ == '__main__':

    #  Run Main Function
    main()

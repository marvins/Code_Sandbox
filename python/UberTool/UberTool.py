#!/usr/bin/env python
#
#    File:    UberTool.py
#    Author:  Marvin Smith
#    Date:    4/18/2014
#

#  System Libraries
import sys, logging

#  PyQt4
from PyQt4 import QtGui

#  Core Files
sys.path.insert(0,'core')
import Preferences

#  GUI Files
sys.path.insert(0,'gui')

#  Main Window
from MainWindow import MainWindow


#-----------------------------#
#-       Main Function       -#
#-----------------------------#
def main():

	#  Load the configuration file
	preferences = Preferences.Preferences( args = sys.argv);

	# Create application 
	app = QtGui.QApplication(sys.argv);

	#  Load the Main Widget
	mainWindow = MainWindow(preferences);

	#  Start the application
	sys.exit(app.exec_());


#-----------------------------#
#-     Start of Program      -#
#-----------------------------#
if __name__ == '__main__':

	#  Run Main Function
	main();


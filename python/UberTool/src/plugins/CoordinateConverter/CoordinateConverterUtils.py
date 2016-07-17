#    File:    CoordinateConverterUtils.py
#    Author:  Marvin Smith
#    Date:    4/23/2014
#
#    Purpose:  Utilities for the CoordinateConverter GUI
#

#  PyQt4
from PyQt4 import QtGui
from PyQt4.QtCore import *


#   Define a set of conversion types we will support
CoordinateTypes=[('Geodetic','Geodetic DD/DM/DMS'),
                 ('UTM','Universal Transverse Mercator')];

#   Define a set of Datums we will support
Datums=[('WGS84','World Geodetic System of 1984'),
        ('WGS72','World Geodetic System of 1972'),
        ('NAD83','North American Datum of 1983')
        ]






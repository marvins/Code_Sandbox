__author__ = 'Marvin Smith'

from .Utilities import Haversine_Distance_Meters

class City(object):

    #  Name of the city
    name = None

    #  Position
    latitude = None
    longitude = None

    #--------------------------------#
    #-         Constructor          -#
    #--------------------------------#
    def __init__(self, name,
                       latitude,
                       longitude):

        #  Set the name
        self.name = name

        #  Set the position
        self.latitude  = latitude
        self.longitude = longitude

    def Distance(cityA, cityB):
        return Haversine_Distance_Meters(cityA.latitude,
                                         cityA.longitude,
                                         cityB.latitude,
                                         cityB.longitude)

    #---------------------------#
    #-      String format      -#
    #---------------------------#
    def __repr__(self):
        return self.name


    def __eq__(self, other):
        return (self.name == other.name)

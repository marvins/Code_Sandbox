__author__ = 'Marvin Smith'

# Python Libraries
import random
from collections import OrderedDict

# Project Libraries
from .Utilities import Haversine_Distance_Meters

class Tour(object):

    #  List of Cities
    cities = []

    #  Distance
    distance = None

    #-----------------------------#
    #-        Constructor        -#
    #-----------------------------#
    def __init__(self, cities = []):

        #  List of cities
        if len(cities) > 0:

            #  Set the list
            self.cities = list(cities)

            #  Compute the fitness
            Tour.Fitness(self)


    def Fitness(tour):

        #  Compute Distance
        tour.distance = 0
        for x in range(0, len(tour.cities)-1):
            tour.distance += Haversine_Distance_Meters(tour.cities[x].latitude,
                                                       tour.cities[x].longitude,
                                                       tour.cities[x+1].latitude,
                                                       tour.cities[x+1].longitude)
        return tour.distance

    def Update_Fitness(self):
        Tour.Fitness(self)

    def Mutate(tour):

        #  Swap two random cities
        anchor01 = random.randint(1, len(tour.cities)-2)
        anchor02 = random.randint(1, len(tour.cities)-2)

        tmp = tour.cities[anchor01]
        tour.cities[anchor01] = tour.cities[anchor02]
        tour.cities[anchor02] = tmp

        tour.Update_Fitness()

    def Crossover(tour1, tour2):

        #  Select two slice points
        anchor01 = random.randint(1, len(tour1.cities)-3)
        anchor02 = random.randint(anchor01+1, len(tour1.cities)-1)


        #  Create outputs
        output1 = Tour()
        output2 = Tour()

        #  Select the subset
        city_list1 = tour2.cities[anchor01:anchor02]
        city_list2 = tour1.cities[anchor01:anchor02]

        #  Reload new values in the other positions
        city_rem1 = list(filter(lambda x: x not in city_list1, tour1.cities))
        city_rem2 = list(filter(lambda x: x not in city_list2, tour2.cities))

        #  Rebuilt the cities
        output1.cities = city_rem1[0:anchor01] + city_list1 + city_rem1[anchor01:]
        output2.cities = city_rem2[0:anchor01] + city_list2 + city_rem2[anchor01:]


        return (output1, output2)

    def __str__(self):

        #  Create output
        output = 'Distance: ' + str(self.distance) + ", Cities: "

        output += self.cities[0].name
        for city in self.cities[1:]:
            output += ', ' + city.name
        return output

    def __repr__(self):

        #  Create output
        output = 'Distance: ' + str(self.distance) + ", Cities: "

        output += self.cities[0].name
        for city in self.cities[1:]:
            output += ', ' + city.name
        return output

    def __eq__(self,other):

        #  Check length
        if len(self.cities) != len(other.cities):
            return False

        #  Iterate over cities
        for x in range(0, len(self.cities)):
            if self.cities[x].name != other.cities[x].name:
                return False

        return True

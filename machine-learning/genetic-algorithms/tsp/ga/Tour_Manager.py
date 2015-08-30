__author__ = 'Marvin Smith'


#  Project libraries
from .Tour import Tour
from .City import City

#  Python Libraries
import itertools, random


class Tour_Manager(object):

    #  List of cities
    cities = None


    #---------------------------------#
    #-          Constructor          -#
    #---------------------------------#
    def __init__(self, cities):

        #  Set the list of cities
        self.cities = cities

    def Build_Population(self, population_size):
        """
        Construct an initial population given the list of cities.
        :param population_size: Size of the desired population
        :return: List of tuples containing each permutation of cities.
        """

        #  Create output list
        population = []

        #  For population size, shuffle
        for i in range(0,population_size):
            temp_tour = self.cities[1:]
            random.shuffle(temp_tour)
            population.append(Tour([self.cities[0]] + temp_tour + [self.cities[0]]))

        #  Return population
        return population
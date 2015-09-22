__author__ = 'Marvin Smith'\

#  Project Libraries
from .Tour import Tour
from .City import City

#  Python Libraries
import itertools, logging


class Greedy_Solver(object):

    #  List of Cities to Search
    cities = None

    #-----------------------#
    #-     Constructor     -#
    #-----------------------#
    def __init__(self, cities):

        #  Set the city list
        self.cities = cities

    #---------------------------------------------#
    #-      Solve using A Greedy DFS-Search      -#
    #---------------------------------------------#
    def Solve(self):

        #  Output tour
        output = Tour()
        output.cities.append(self.cities[0])

        #  City Queue
        city_list = self.cities[1:]

        #  Start with the first city
        current_city = self.cities[0]
        current_dist = 0

        #  Iterate until we have used up all cities
        while len(city_list) > 0:

            #  Candidate values
            candidate_idx  = 0
            candidate_dist = City.Distance(current_city,city_list[candidate_idx])

            #  Iterate over all cities, comparing the distance
            for x in range(1, len(city_list)):

                #  Compute the dist
                test_dist = City.Distance(current_city,city_list[x])
                if test_dist < candidate_dist:
                    candidate_dist = test_dist
                    candidate_idx  = x

            #  Add the city
            output.cities.append(city_list[candidate_idx])

            #  Remove city from list
            city_list.remove(city_list[candidate_idx])


        #  Update the fitness
        output.Update_Fitness()

        return output

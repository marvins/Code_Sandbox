__author__ = 'Marvin Smith'

#  Project Libraries
from .Tour import Tour

#  Python Libraries
import itertools, logging


class Brute_Force_Solver(object):

    cities = None

    def __init__(self, cities):

        self.cities = cities


    def Solve(self):

        counter = 0
        best_perm    = list(self.cities) + [self.cities[0]]
        best_fitness = Tour.Fitness(Tour(best_perm))

        #  Run through each permutation
        for perm in itertools.permutations(self.cities[1:]):
            counter += 1
            city_list = [self.cities[0]] + list(perm) + [self.cities[0]]

            if counter % 1000000 == 0:
                logging.debug('Iteration: ' + str(counter) + ', Best Result: ' + str(Tour(best_perm)))


            if Tour(city_list).distance < best_fitness:
                best_fitness = Tour.Fitness(Tour(city_list))
                best_perm    = city_list


        solution = Tour(list(best_perm))

        return solution
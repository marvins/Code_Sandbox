#!/usr/bin/env python3
__author__ = 'Marvin Smith'

#  Python Libraries
import argparse, csv, math, numpy as np, time, logging

#  Project Libraries
import ga
from ga import City, Tour
from ga.Greedy_Solver import Greedy_Solver
from ga.Genetic_Algorithm import Genetic_Algorithm
from ga.Tour_Manager import Tour_Manager
from ga.Utilities import Write_Output
from ga.Brute_Force_Solver import Brute_Force_Solver

#----------------------------------#
#-        Load TSP Dataset        -#
#----------------------------------#
def Load_Dataset(options):

    #  Output Results
    cities = []
    counter = 0

    #  Load the csv file
    with open( options.input_dataset) as csvfile:
        tsp_reader = csv.DictReader(csvfile)
        for row in tsp_reader:
            cities.append( City.City(name=row['Name'],
                                     latitude=float(row['Latitude']),
                                     longitude=float(row['Longitude'])))
            counter += 1


    return cities



#------------------------------------#
#-        Parse Command-Line        -#
#------------------------------------#
def Parse_Command_Line():

    #  Create parser
    parser = argparse.ArgumentParser(description='Solve the Traveling Salesman Problem.')


    #   Set the input dataset
    parser.add_argument('-i','--input',
                        dest='input_dataset',
                        required=True,
                        help='Dataset to solve for.')

    #   Set the population size
    parser.add_argument('-p','--pop-size',
                        dest='population_size',
                        required=False,
                        default=1000,
                        help='Set the initial population size.')

    #  Run a maximum number of iterations
    parser.add_argument('-m','--max-iterations',
                        dest='max_iterations',
                        default=100000,
                        required=False,
                        help='Set the max number of times to run.')

    parser.add_argument('-mr','--mutation-rate',
                        dest='mutation_rate',
                        default=0.15,
                        required=False,
                        type=float,
                        help='Set the mutation rate.')

    parser.add_argument('-sr','--selection-rate',
                        dest='selection_rate',
                        default=0.5,
                        required=False,
                        type=float,
                        help='Set the selection rate.')

    parser.add_argument('-pr','--preservation-rate',
                        dest='preservation_rate',
                        default=0.01,
                        required=False,
                        type=float,
                        help='Set the preservation rate.')

    parser.add_argument('-er','--exit-on-repeats',
                        dest='exit_on_repeats',
                        default=200,
                        required=False,
                        type=int,
                        help="Set the number of times the best value is repeated before exiting.")

    parser.add_argument('-v','--verbose',
                        dest='verbosity',
                        default='INFO',
                        action='store_const',
                        const='DEBUG',
                        help='Enable verbose logging.')
    #  Parse and return
    return parser.parse_args()


#---------------------#
#-       Main        -#
#---------------------#
def Main():

    #  Parse Command-Line Options
    options = Parse_Command_Line()

    logging.basicConfig(level=getattr(logging,options.verbosity))

    #  Load the dataset
    cities = Load_Dataset(options)

    #  Build the Initial Population
    population = Tour_Manager(cities).Build_Population(options.population_size)


    #  Construct Genetic Algorithm
    ga = Genetic_Algorithm(population=population,
                           fitness_function=lambda Tour: Tour.distance,
                           crossover_algorithm=Tour.Tour.Crossover,
                           selection_rate=options.selection_rate,
                           mutation_algorithm=Tour.Tour.Mutate,
                           mutation_rate=options.mutation_rate,
                           preservation_rate=options.preservation_rate,
                           exit_on_repeats=options.exit_on_repeats)


    #  Run N Times
    now = time.time()
    logging.debug('Solving with Genetic Algorithm')
    population = ga.Run_Iterations(options.max_iterations)
    ga_time = time.time()-now

    #  Solve using greedy
    now = time.time()
    logging.debug('Solving with Greedy Algorithm')
    gd_solution = Greedy_Solver(cities).Solve()
    gd_time = time.time()-now

    #  Solve using brute-force
    now = time.time()
    logging.debug('Solving with Brute Force Algorithm')
    bf_solution = Brute_Force_Solver(cities).Solve()
    bf_time = time.time() -now

    #  Check if the Brute-Force Solution is the same or backwards
    if not (population[0] == bf_solution):
        if (population[0] == Tour.Tour(bf_solution.cities.reverse())):
            bf_solution = Tour.Tour(bf_solution.cities.reverse())
        else:
            logging.warning('solutions are different!')


    #  Print onto the web-page
    Write_Output(population[0],
                 bf_solution,
                 gd_solution)

    logging.info('GA Runtime: ' + str(ga_time) + ' seconds')
    logging.info('GD Runtime: ' + str(gd_time) + ' seconds')
    logging.info('BF Runtime: ' + str(bf_time) + ' seconds\n')
    logging.info('GA Best Solution: ' + str(population[0]))
    logging.info('GD Best Solution: ' + str(gd_solution))
    logging.info('BF Best Solution: ' + str(bf_solution))



if __name__ == '__main__':
    Main()

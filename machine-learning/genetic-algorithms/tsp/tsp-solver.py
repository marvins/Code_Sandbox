#!/usr/bin/env python3
__author__ = 'Marvin Smith'

#  Python Libraries
import argparse, csv, math, numpy as np, time, logging

#  Project Libraries
import ga
from ga import City, Tour
from ga.Configuration import Configuration
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
    with open( options.general_params['input_dataset']) as csvfile:
        tsp_reader = csv.DictReader(csvfile)
        for row in tsp_reader:
            cities.append( City.City(name=row['Name'],
                                     latitude=float(row['Latitude']),
                                     longitude=float(row['Longitude'])))
            counter += 1

    #  Check if we need to remove any cities
    if options.general_params['skip_cities'] > 0:
        cities = cities[0:(len(cities)-options.general_params['skip_cities'])]
    logging.debug('loaded ' + str(len(cities)) + " cities")

    return cities


#-----------------------------------------#
#-      Run the Genetic Algorithm        -#
#-----------------------------------------#
def Run_GA( options, cities ):

    #  Log
    logging.debug('Running Genetic Algorithm')

    #  Build the Initial Population
    population = Tour_Manager(cities).Build_Population(options.ga_params['population_size'])


    #  Construct Genetic Algorithm
    ga = Genetic_Algorithm(population=population,
                           fitness_function=lambda Tour: Tour.distance,
                           crossover_algorithm=Tour.Tour.Crossover,
                           selection_rate=options.ga_params['selection_rate'],
                           mutation_algorithm=Tour.Tour.Mutate,
                           mutation_rate=options.ga_params['mutation_rate'],
                           preservation_rate=options.ga_params['preservation_rate'],
                           exit_on_repeats=options.ga_params['exit_on_repeats'])



    #  Run N Times
    now = time.time()
    logging.debug('Solving with Genetic Algorithm')
    population = ga.Run_Iterations(options.ga_params['max_iterations'])
    ga_time = time.time()-now

    #  Log the Time
    logging.info('GA Runtime: ' + str(ga_time) + ' seconds')

    #  Return results
    return {'time':ga_time,
            'solution':population[0],
            'population_size':len(population),
            'selection_rate':options.ga_params['selection_rate']}



#----------------------------------------#
#-      Run the Greedy Algorithm        -#
#----------------------------------------#
def Run_GD( options, cities):

    #  Log
    logging.debug('Running Greedy Algorithm')

    #  Solve using greedy
    now = time.time()
    gd_solution = Greedy_Solver(cities).Solve()
    gd_time = time.time()-now

    #  Log the Time
    logging.info('GD Runtime: ' + str(gd_time) + ' seconds')

    return {'solution':gd_solution,
            'time':gd_time}


#---------------------------------------------#
#-      Run the Brute Force Algorithm        -#
#---------------------------------------------#
def Run_BF( options, cities):

    #  Log
    logging.debug('Running Brute Force Algorithm')

    #  Start algorithm
    now = time.time()
    bf_solution = Brute_Force_Solver(cities).Solve()
    bf_time = time.time() -now

    #  Log the Time
    logging.info('BF Runtime: ' + str(bf_time) + ' seconds')

    return {'time':bf_time,
            'solution':bf_solution,
            'dist':0}


#---------------------#
#-       Main        -#
#---------------------#
def Main():

    #  Parse Command-Line Options
    options = Configuration()


    #  Configure Logging
    logging.basicConfig(level=getattr(logging,
                                      options.general_params['log_level']))


    #  Load the dataset
    cities = Load_Dataset(options)
    ga_results = {}
    gd_results = {}
    bf_results = {}


    #  Run the Genetic Algorithm
    if options.general_params['skip_ga'] is False:
        ga_results = Run_GA(options, cities)


    #  Run the Greedy Algorithm
    if options.general_params['skip_gd'] is False:
        gd_results = Run_GD(options, cities)


    #  Run the Brute-force Algorithm
    if options.general_params['skip_bf'] is False:
        bf_results = Run_BF(options, cities)


    #  Check if the Brute-Force Solution is the same or backwards
    if options.general_params['skip_bf'] is False and options.general_params['skip_ga'] is False:
        if not (ga_results['solution'] == bf_results['solution']):
            if (ga_results['solution'] == Tour.Tour(bf_results['solution'].cities.reverse())):
                bf_results['solution'] = Tour.Tour(bf_results['solution'].cities.reverse())
            else:
                logging.warning('solutions are different!')


    #  Compute distances
    if options.general_params['skip_ga'] is False and options.general_params['skip_bf'] is False:
        ga_results['dist'] = ga_results['solution'].distance - bf_results['solution'].distance
        logging.info('GA Best Solution: ' + str(ga_results['solution']))


    if options.general_params['skip_gd'] is False and options.general_params['skip_bf'] is False:
        gd_results['dist'] = gd_results['solution'].distance - bf_results['solution'].distance
        logging.info('GD Best Solution: ' + str(gd_results['solution']))

    if options.general_params['skip_bf'] is False:
        logging.info('BF Best Solution: ' + str(bf_results['solution']))

    #  Print onto the web-page
    if options.general_params['skip_html'] is False:
        Write_Output(ga_results,
                     gd_results,
                     bf_results)



    # Append Performance Info to Filename
    options.Update_Performance_Information( ga_info=ga_results,
                                            gd_info=gd_results,
                                            bf_info=bf_results,
                                            number_cities=len(cities))

if __name__ == '__main__':
    Main()

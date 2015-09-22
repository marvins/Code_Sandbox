__author__ = 'Marvin Smith'

#  Python Libraries
import random, math, logging


class Exit_Condition(object):

    max_repeats      = None
    repeated_counter = 0
    current_best     = None

    def __init__(self, max_repeats = 100):

        #  Set the max repeat value
        self.max_repeats = max_repeats


    def Check_Exit(self, test_fitness):
        """
        Evaluate the candidate item to see if we have reached the desired exit condition
        :param candidate:
        :return:
        """

        #  Check if we have even started yet
        if self.current_best is None:
            self.current_best     = test_fitness
            self.repeated_counter = 0
            return False

        #  If the candidate and best are equivalent
        elif math.fabs(test_fitness - self.current_best) < 1:
            self.repeated_counter+= 1
            if self.repeated_counter >= self.max_repeats:
                return True
            else:
                return False

        #  Otherwise, they are not the same
        else:
            self.repeated_counter = 0
            if self.current_best < test_fitness:
                logging.debug("warning: replacing the current exit test with an inferior result")
            self.current_best     = test_fitness
            return False


class Genetic_Algorithm(object):

    #  Current Population
    population = None

    #  Fitness Function
    fitness_function = None

    #  Selection Rate
    selection_rate = None

    #  Crossover Algorithm
    crossover_algorithm = None

    #  Mutation Algorithm
    mutation_algorithm = None

    #  Mutation Rate
    mutation_rate = None

    #  Preservation rate
    preservation_rate = None

    #  Exit Object
    exit_object = None

    #----------------------------#
    #-        Constructor       -#
    #----------------------------#
    def __init__(self, population,
                       fitness_function,
                       crossover_algorithm,
                       mutation_algorithm,
                       selection_rate=0.25,
                       mutation_rate=0.02,
                       preservation_rate=0.01,
                       exit_on_repeats=1000):

        self.exit_object = Exit_Condition(exit_on_repeats)

        #  Set the population
        self.population = population

        #  Set the fitness function
        self.fitness_function = fitness_function

        #  Set the selection_rate
        self.selection_rate = selection_rate

        #  Set the crossover algorithm
        self.crossover_algorithm = crossover_algorithm

        #  Set the mutation algorithm
        self.mutation_algorithm = mutation_algorithm

        #  Set the mutation rate
        self.mutation_rate = mutation_rate

        #  Set the preservation rate
        self.preservation_rate = preservation_rate



    def Run_Iterations(self, max_iterations):

        #  Size of the subset
        subset_size = int(self.selection_rate * len(self.population))
        preserve_size = int(self.preservation_rate * len(self.population))
        mutation_size = int(self.mutation_rate * len(self.population))

        #  Population size
        pop_size = len(self.population)

        #  For each iteration
        for i in range(0, max_iterations):

            #  Select Preservation Set
            preservation_list = self.population[0:preserve_size]

            #  Run Selection
            sub_population = self.population[0:subset_size]

            #  Create result population
            self.population = preservation_list + sub_population

            #  Run Crossover
            while len(self.population) < pop_size:

                #  Select two indeces
                idA = random.randint(0, subset_size-1)
                idB = random.randint(0, subset_size-1)

                #  Merge two samples
                samples = self.crossover_algorithm(sub_population[idA],
                                                   sub_population[idB])

                #  Add the samples
                if len(sub_population) < pop_size:
                    self.population.append(samples[0])
                if len(sub_population) < pop_size:
                    self.population.append(samples[1])


            #  Run Mutation
            for x in range(preserve_size,mutation_size):

                #  Select a random index
                idx = random.randint(preserve_size, len(self.population)-1)
                self.mutation_algorithm(self.population[idx])


            #  Recompute fitness
            for x in range(0,len(self.population)):
                self.population[x].Update_Fitness()


            #  Sort
            self.population = sorted(self.population,
                                     key=self.fitness_function)


            #  Exit Condition
            if self.exit_object.Check_Exit(self.population[0].distance) == True:
                logging.debug('Exiting early on exit condition.')
                break

        #  Return the population
        return self.population
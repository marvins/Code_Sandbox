#ifndef __SRC_GENETIC_ALGORITHM_HPP__
#define __SRC_GENETIC_ALGORITHM_HPP__

#include <deque>
#include <utility>
#include <vector>

#include "Fitness_Functor.hpp"

namespace GA{

std::vector<bool> generate_genome( const int& sz );

class GA{

    public:
        
        GA( const Fitness_Functor& fit_func, const int& num_bits, const int& pop_size, const int& preserve_cnt, const double& sel_rate );
        
        /**
         * Perform Selection
         * 1. Apply fitness function on data
         * 2. Sort data by fitness
         * 3. Select elites to be preserved
         * 4. Select the remaining survivors
         */
        void selection( );

        /**
         * Perform Crossover
        */
        void crossover( );

        /**
         * Perform Mutation
        */
        void mutation( );


        void print()const;

    private:

        /** This contains our genetic material */
        std::deque<std::pair<std::vector<bool>, double> > population;
        
        /** This contains our preserved elites */
        std::deque<std::pair<std::vector<bool>, double> > elites;

        /** This contains the survivors for the round */
        std::deque<std::pair<std::vector<bool>, double> > survivors;
        
        /** This is the expected number of bits for each string */
        const int number_bits;

        /** This is the maximum size of the population */
        const int population_size;

        /** This is the functor used to evaluate the fitness */
        const Fitness_Functor fitness_func;

        /** This is the number of items saved each round */
        const int preservation_count;

        /** This is the selection rate for the GA */
        const double selection_rate;
};


}


#endif


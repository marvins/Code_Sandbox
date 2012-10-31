#include "Genetic_Algorithm.hpp"

#include <algorithm>
#include <iostream>

using namespace std;

namespace GA{

std::vector<bool> generate_genome( const int& sz ){
    
    vector<bool> output;
    for( int i=0; i<sz; i++ )
        output.push_back( rand()%2 );
    return output;
}

GA::GA( const Fitness_Functor& fit_func, const int& num_bits, const int& pop_size, const int& preserve_cnt ): 
    number_bits(num_bits), 
    population_size(pop_size),
    fitness_func( fit_func ),
    preservation_count( preserve_cnt)
    {

    //initialize the population
    for( int i=0; i<population_size; i++ )
        population.push_back( pair<vector<bool>,double>( generate_genome( MAX_GENOME_LENGTH ), 0 ) );
    

}


void GA::selection( ){
    
    // for every item in the population, measure the fitness
    for( size_t i=0; i<population.size(); i++ ){
        population[i].second = fitness_func( population[i].first );
    }
    
    // sort by the fitness
    sort( population.begin(), population.end(), Fitness_Func_Sort( ));

    // preserve the first set of items


}


void GA::print()const{
    
    cout << "Size: " << population.size() << endl;
    cout << "First 10 Strings" << endl;
    for( int i=0; i<10; i++ ){
        cout << "Item: " << population[i].second << ", Data: ";
        for( size_t j=0; j<population[i].first.size(); j++ )
            cout << population[i].first[j];
        cout << endl;
    }

}


}


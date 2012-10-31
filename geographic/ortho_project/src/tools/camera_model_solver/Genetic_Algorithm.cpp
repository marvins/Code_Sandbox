#include "Genetic_Algorithm.hpp"

#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>

using namespace std;

namespace GA{

std::vector<bool> generate_genome( const int& sz ){
    
    vector<bool> output;
    for( int i=0; i<sz; i++ )
        output.push_back( rand()%2 );
    return output;
}


vector<bool> uniform_crossover( vector<bool>const& a, vector<bool>const& b ){
    
    if( a.size() != b.size() ) throw string("ERROR: both items are different sizes");
    
    vector<bool> out(a.size());
    for( size_t i=0; i<a.size(); i++ ){
        
        if( (int)(rand()%2) == 0 )
            out[i] = a[i];
        else
            out[i] = b[i];
    }
    
    return out;
}


GA::GA( const Fitness_Functor& fit_func, const int& num_bits, const int& pop_size, const int& preserve_cnt, const double& sel_rate ): 
    number_bits(num_bits), 
    population_size(pop_size),
    fitness_func( fit_func ),
    preservation_count( preserve_cnt),
    selection_rate( sel_rate )
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

    
    // preserve our elites
    elites.clear();
    for( int i=0; i<preservation_count; i++ )
        elites.push_back( population[i] );

    // start choosing from the rest
    survivors.clear();
    for( size_t i=0; i<(population.size()*selection_rate); i++ ){
        
        // select a random index
        int idx = rand()%population.size();
        survivors.push_back( population[idx] );
    }


}


void GA::crossover( ){
    
    // add the elites to the new population
    population.clear();
    for( size_t i=0; i<elites.size(); i++ )
        population.push_back( elites[i] );


    // for the remaining population items, minus the elites, choose pairs
    int num_items = population_size - elites.size();
    for( int i=0; i<num_items; i++ ){
        
        // choose first index
        int idxA = rand()%survivors.size();

        // choose second index
        int idxB = rand()%survivors.size();
        
        // add the uniform cross
        population.push_back( pair<vector<bool>, double>( uniform_crossover( survivors[idxA].first, survivors[idxB].first  ), 0));

    }
    
    if( population.size() != population_size )
        throw string("ERROR: Sizes are not uniform");

}


void GA::mutation( ){

    // perform random mutation
    for( size_t i=preservation_count; i<population.size(); i++ )
        for( size_t j=0; j<population[i].first.size(); j++ )
            if( rand()%1000 > 5 )
                population[i].first[j] = (bool)((int)rand()%2);

}


void GA::print()const{
    
    cout << endl << endl;
    cout << "---------------------------------------------------------------------------" << endl;
    cout << "First 10 Strings" << endl;
    for( int i=0; i<10; i++ ){
        cout << "Item: " << population[i].second << ", Data: ";
        for( size_t j=0; j<population[i].first.size(); j++ )
            cout << population[i].first[j];
        cout << endl;
    }
    ofstream fout;
    fout.open("best.txt");
    fitness_func.print_vars( fout, population[0].first);
    fitness_func.print_vars( cout, population[0].first);
    fout.close();
}


}


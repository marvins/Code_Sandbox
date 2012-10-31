#include "Genetic_Algorithm.hpp"

#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>

#include <GeoImage.hpp>

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


vector<bool> single_point_crossover( vector<bool>const& a, vector<bool>const& b ){

    if( a.size() != b.size() ) throw string("ERROR: both items are different sizes");
    
    vector<bool> out(a.size());
    
    //select a single index value to use as the pivot point
    int idx = rand()%a.size();
    for( size_t i=0; i<a.size(); i++ ){
        
        if( (int)i < idx )
            out[i] = a[i];
        else
            out[i] = b[i];
    }
    
    return out;
}


vector<bool> two_point_crossover( vector<bool>const& a, vector<bool>const& b ){

    if( a.size() != b.size() ) throw string("ERROR: both items are different sizes");
    
    vector<bool> out(a.size());
    
    //select a single index value to use as the pivot point
    int idxA = rand()%(a.size()-1);
    int idxB = rand()%(a.size()-idxA);

    for( size_t i=0; i<a.size(); i++ ){
        
        if( (int)i < idxA && (int)i < idxB)
            out[i] = a[i];
        else if( (int)i < idxB )
            out[i] = b[i];
        else
            out[i] = a[i];
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
        int idx = rand()%(int)(population.size()*selection_rate);
        survivors.push_back( population[idx] );
    }


}


void GA::crossover( ){
   
    int number_refresh = 100;

    // add the elites to the new population
    population.clear();
    for( size_t i=0; i<elites.size(); i++ )
        population.push_back( elites[i] );


    // for the remaining population items, minus the elites, choose pairs
    int num_items = population_size - elites.size();
    for( int i=0; i<num_items-number_refresh; i++ ){
        
        // choose first index
        int idxA = rand()%survivors.size();

        // choose second index
        int idxB = rand()%survivors.size();
        
        // add the uniform cross
        population.push_back( pair<vector<bool>, double>( two_point_crossover( survivors[idxA].first, survivors[idxB].first  ), 0));

    }

    // always add 5 random strings as well
    for( int i=0; i<number_refresh; i++ )
        population.push_back( pair<vector<bool>,double>( generate_genome(number_bits), 0));

    
    if( (int)population.size() != population_size )
        throw string("ERROR: Sizes are not uniform");

}


void GA::mutation( ){

    // perform random mutation
    for( size_t i=preservation_count; i<population.size(); i++ )
        for( size_t j=0; j<population[i].first.size(); j++ )
            if( rand()%1000 < 5 )
                population[i].first[j] = (bool)((int)rand()%2);

}


double GA::best_fitness( )const{
    return population[0].second;
}

void GA::print()const{
    
    system("clear");
    cout << "Current Genetic Algorithm Progress" << endl;
    cout << "---------------------------------------------------------------------------" << endl;
    cout << "First 20 of " << population.size() << " Strings" << endl;
    for( int i=0; i<20; i++ ){
        cout << "Fitness: ";
        printString( cout, GEO::STR::num2str(population[i].second), 10, "LEFT");
        cout << ", Genotype: ";
        for( size_t j=0; j<population[i].first.size(); j++ )
            cout << population[i].first[j];
        cout << endl;
    }
    cout << endl;
    cout << "---------------------------------------------------------------------------" << endl;
    cout << population.size()-100 << " through " << population.size()-90 << " Strings" << endl;
    for( int i=population.size()-100; i<population.size()-90; i++ ){
        cout << "Fitness: ";
        printString( cout, GEO::STR::num2str(population[i].second), 10, "LEFT");
        cout << ", Genotype: ";
        for( size_t j=0; j<population[i].first.size(); j++ )
            cout << population[i].first[j];
        cout << endl;
    }
    cout << endl;
    cout << "Current Best Fit" << endl;
    ofstream fout;
    fout.open("best.txt");
    
    for( size_t i=0; i<20; i++ )
        fitness_func.print_vars( fout, population[i].first);
    fitness_func.print_vars( cout, population[0].first);
    fout.close();
}


}


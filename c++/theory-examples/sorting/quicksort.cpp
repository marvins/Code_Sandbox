/**
 * @file    quicksort.cpp
 * @author  Marvin Smith
 * @date    4/10/2014
 */
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

/**
 * Swap
 */
template<typename TP>
void swapData( TP& itemA, TP& itemB ){
    TP tempItem = itemA;
    itemA = itemB;
    itemB = tempItem;
}

/**
 * Partition
 */
int partition( std::vector<double>& data, int const& leftIdx, int const& rightIdx, int const& pivot ){
    
    // pivot value
    double pivotValue = data[pivot];
    
    // swap the pivot and the last item
    swapData( data[pivot], data[rightIdx]);

    // create temp container to store our swap value
    int swapIndex = leftIdx;

    // begin sorting left and right sides
    for( int i=leftIdx; i<= (rightIdx-1); i++ ){
        
        if( data[i] <= pivotValue ){
            
            // swap elements
            swapData( data[i], data[swapIndex] );

            // increment the swap index
            swapIndex++;
        }
    }

    // return the swapped value
    swapData( data[swapIndex], data[rightIdx]);

    return swapIndex;
}


/**
 * In-Place Quicksort Algorithm
 */
void quicksort( std::vector<double>& data, int const& leftIndex, int const& rightIndex ){
    
    // skip if the left and right indeces are opposite
    if( leftIndex >= rightIndex ){
        return;
    }

    // make sure the array is not empty or 1 element
    if( data.size() <= 1 ){ return; }

    // select a random pivot point
    int pivot = rand()%(rightIndex - leftIndex) + leftIndex;

    // call partition
    int newPivot = partition( data, leftIndex, rightIndex, pivot );
    
    // call quicksort with new partition
    quicksort( data, leftIndex, newPivot -1 );
    quicksort( data, newPivot+1,   rightIndex ); 
}


/**
 * Print data to the console
 */
void print_data( std::vector<double> const& data ){

    std::vector<double>::const_iterator it  = data.begin();
    std::vector<double>::const_iterator eit = data.end();
    for( ; it != eit; it++ ){
        cout << (*it) << ", ";
    }
    cout << endl;

}

/**
 * Main Function
 */
int main( int argc, char* argv[] ){
    
    // seed the generator
    srand(time(NULL));

    // create a container to sort
    std::vector<double> data;
    //for(size_t i=0; i<10; i++ ){
    //    data.push_back( rand()%100);
    //}
    data.push_back(3);
    data.push_back(4);
    data.push_back(2);
    data.push_back(1);
    data.push_back(5);
    data.push_back(7);
    data.push_back(9);
    data.push_back(8);
    data.push_back(5);

    // print first array
    print_data( data );

    // do in place quicksort
    quicksort( data, 0, data.size()-1 );

    // print sorted array
    print_data( data );


    return 0;
}


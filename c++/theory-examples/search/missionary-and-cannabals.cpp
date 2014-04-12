#include <deque>
#include <iostream>
#include <set>
#include <string>

using namespace std;


/**
 * State Container
 */
class State{

    public:
        
        /**
         * Constructor
         */
        State( const int& m=0, const int& c=0, const bool& hasBoat=false, const int& l=0) : miss(m), cann(c), boatLeft(hasBoat), level(l){}
    
        bool operator == ( const State& rhs )const{
            
            if( boatLeft == rhs.boatLeft ){
                if( miss == rhs.miss ){
                    return cann == rhs.cann;
                }
                else{
                    return false;
                }
            }
            else{
                return false;
            }
        }

        bool operator < ( const State& rhs )const{ 
            if( boatLeft == rhs.boatLeft ){
                if( miss == rhs.miss ){
                    return (cann < rhs.cann );
                }
                else {
                    return (miss < rhs.miss);
                }
            }
            else
                return boatLeft < rhs.boatLeft;
        }

        
        
        int miss;
        int cann;
        bool boatLeft;
        int level;

}; /// End of State Class


bool stateIsValid( State const& state ){
    
    // make sure the boat is not empty
    if( state.miss < 0 && state.cann < 0 ){
        return false;
    }

    // make sure the cannibals don't outnumber the missionaries
    if( (   state.miss  > 0) && (   state.miss  <    state.cann) ){ return false; }
    if( ((3-state.miss) > 0) && ((3-state.miss) < (3-state.cann))){ return false; }
    
    // make sure the number of missionaries and cannabals don't exceed
    if( state.miss > 3 || state.cann > 3 || state.miss < 0 || state.cann < 0 ){
        return false;
    }

    return true;

}

string boatSide( const bool& boat ){

    if( boat == true ){
        return "left";
    } else {
        return "right";
    }
}

/**
 * Main Function
 */
int main( int argc, char* argv[] ){

    
    std::set<State> visited_set;
    
    // create a root node with a starting condition
    State currentState(3, 3, true, 0);
    State tempNode;

    // create our queue to manage searching
    deque<State> state_list;
    state_list.push_back(currentState);


    // start iterating over the queue until we either run out
    // of nodes or find the solution.
    while( state_list.size() > 0 ){

        // extract the next node
        State currentNode = state_list[0];
        state_list.pop_front();
        
        cout << "Level: " << currentNode.level << ", Missionaries: " << currentNode.miss << ", Cannabals: " << currentNode.cann << ", Side: " << boatSide(currentNode.boatLeft) << endl;
        // check if we have a solution
        if( currentNode.miss == 0 && currentNode.cann == 0 ){
            cout << "Solution Found" << endl;
            break;
        }

        // otherwise, we need to generate follow-on actions
        else{
            
            // create an action for each combo of people
            for( int m=0; m<=2; m++ ){
            for( int c=0; c<=2; c++ ){
                    
                // make sure the boat is not empty
                if(((m+c) > 0) && ((m+c) <= 2)){
                    
                    // if we have the boat, then we subtract
                    if( currentNode.boatLeft == true ){
                        tempNode.miss = currentNode.miss - m;
                        tempNode.cann = currentNode.cann - c;
                        tempNode.boatLeft = false;
                    }
                    // otherwise, we add
                    else{
                        tempNode.miss = currentNode.miss + m;
                        tempNode.cann = currentNode.cann + c;
                        tempNode.boatLeft = true;
                    }
                    tempNode.level = currentNode.level + 1;

                    //make sure the node is valid
                    if( stateIsValid( tempNode ) == true ){
                        
                        // make sure we have not already visited
                        // it
                        if( visited_set.find(tempNode) == visited_set.end()){
                            state_list.push_back(tempNode);
                            visited_set.insert(tempNode);
                    }
                    }

                }
            }}

        }

    }

    return 0;
}


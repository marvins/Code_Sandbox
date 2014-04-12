/**
 * @file    mergesort.cpp
 * @author  Marvin Smith
 * @date    4/11/2014
 */
#include <iostream>

#include <boost/shared_ptr.hpp>


using namespace std;

// global counter to ensure sort is preserved
int global_counter = 0;

template<typename TP>
class Node_{
    
    public:
        
        /// Define our own type
        typedef TP datatype;

        /// Define our own pointer type
        typedef boost::shared_ptr<Node_<datatype> > ptr_t;

        /**
         * Default Constructor
         */
        Node_( datatype const& data ) : m_data(data), m_count(global_counter++), m_next(nullptr){};

        /// Data variable
        datatype m_data;
        
        /// Counter variable
        int m_count;

        /// Pointer to next element
        boost::shared_ptr<Node_<datatype> > m_next;
           

}; // End of Node class

typedef Node_<double> Node;


class List{

    public:
    
        /// Pointer type
        typedef boost::shared_ptr<List> ptr_t;

        /**
         * Default constructor
         */
        List() : m_head(nullptr), m_tail(nullptr), m_size(0){}

        /**
         * Add an element
         */
        void push( Node::datatype const& item ){
            
            // create a node;
            Node::ptr_t node(new Node(item));
           
            // if this is the first node, this is the head
            if( size() == 0 ){
                m_head = node;
                m_tail = m_head;
            }

            // otherwise, move the tail to our new node
            else{
                m_tail->m_next = node;
                m_tail = m_tail->m_next;
            }

            // increment the size
            m_size++;
        }

        /**
         * Print the list
         */
        void print()const{
            
            // create a reference  
            boost::shared_ptr<Node> head = m_head;
            while( head != nullptr ){
                
                cout << "( " << head->m_data << ", " << head->m_count << " )";
                if( head->m_next != nullptr ){
                    cout << ", ";
                }

                if( head == m_head ){ cout << "<-head"; }
                if( head == m_tail ){ cout << "<-tail"; }
                head = head->m_next;
                
                cout << endl;
            }
            cout << endl;

        }

        /**
         * Sort and re-merge data
         */
        Node::ptr_t sortedMerge( Node::ptr_t& a, Node::ptr_t& b ){
            
            // create an output pointer
            Node::ptr_t outputHead = nullptr;

            // return one or the other if one is null
            if( a == nullptr ){
                m_tail = b;
                return b;
            } else if( b == nullptr ){
                m_tail = a;
                return a;
            }

            // otherwise, pick one our the other and redo
            if( a->m_data <= b->m_data ){
                outputHead = a;
                outputHead->m_next = sortedMerge( a->m_next, b);
            }
            else{
                outputHead = b;
                outputHead->m_next = sortedMerge( a, b->m_next );
            }

            return outputHead;

        }

        /**
         * Do Merge Sort
         */
        void mergeSort(){
        
            mergeSortImpl( m_head, size() );

        }

        void mergeSortImpl( Node::ptr_t& headNode, const size_t& subListSize ){

            // skip if the list is empty or has one item
            if( headNode == nullptr || headNode->m_next == nullptr ){
                return;
            }

            // split the list into two halves
            Node::ptr_t a, b;
            Node::ptr_t head = headNode;

            size_t aSize, bSize;

            this->split( head, a, b, subListSize, aSize, bSize );
            
            // merge sort each sublist again
            mergeSortImpl( a, aSize );
            mergeSortImpl( b, bSize );

            //sort and merge back together
            headNode = sortedMerge( a, b );

        }

        /**
         * Split the list
         */
        void split( Node::ptr_t& head, Node::ptr_t& a, Node::ptr_t& b, size_t const& subListSize, size_t& aSize, size_t& bSize ){

            // set node a to the first position
            a = head;

            // set b to halfway
            b = head;
            size_t halfSize = subListSize/2;
            if( subListSize % 2 == 1 ){ halfSize++; }
            for( size_t i=0; i<(halfSize-1); i++ ){
                b = b->m_next;
            }

            // increment pointer, then delete mid references
            Node::ptr_t temp = b;
            b = b->m_next;
            temp->m_next = nullptr;
            
            // return the array sizes
            aSize = halfSize;
            bSize = subListSize - aSize;

        }

        
        /**
         * Clear the list
         */
        void clear(){
            cout << "clear list" << endl;
        }

        /**
         * Get the size of the list
         */
        size_t size()const{
            return m_size;
        }

        // pointer to first element
        Node::ptr_t m_head;

        // pointer to last element
        Node::ptr_t m_tail;

        // current size of list
        size_t m_size;
    

}; // End of List class



/**
 * Generate a random list
 */
List::ptr_t generate_list( const int& numItems ){

    // create an empty list
    List::ptr_t list( new List );

    // create N random items
    for( int i=0; i<numItems; i++ ){
        list->push( rand()%100 );
    }

    return list;
}


int main( int argc, char* argv[] ){

    // seed our random number generator
    srand(time(NULL));

    // create linked list of temporary data
    List::ptr_t list = generate_list(100);

    // print the list
    list->print();

    // sort the list
    list->mergeSort();

    // print the new list
    list->print();

    return 0;
}



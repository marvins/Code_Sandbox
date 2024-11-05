/**
 * @file    main.cpp
 * @author  Marvin Smith
 * @date    11/3/2024
 */

// C++ Standard Libraries
#include <iostream>
#include <thread>

// Memory Tracker Libraries
#include <memtracker/MemoryTracker.hpp>
#include <memtracker/ThreadSafeQueue.hpp>

enum class TypesToTrack : int32_t {
    CLASS_1 = 0,
    CLASS_2 = 1,
}; // End of TypesToTrack

/**
 * This class has a few unique MemoryTracker-specific needs
 * You need to tell 
 */
class Class1
{
    public:

        Class1(){

            // We need the size of this class
            size_t mem_size = m_data.size() * sizeof(double);

            // We need this class's unique ID
            auto class_id = static_cast<int>(TypesToTrack::CLASS_1);

            // 
            //mem::MemoryTracker
        }

        static Class1 create() {
            return Class1();
        }

    private:

        // A randomly-allocated chunk of data
        std::array<double,20> m_data { 0 };
}; 

class Class2
{
    public:

        Class2();


        static Class2 create() {
            return Class2();
        }

};

/**
 * Primary Creation Thread
 */
template <typename ValueType>
void create_objects( std::chrono::system_clock::time_point  end_at,
                 std::chrono::milliseconds              sleep_time,
                 mem::ThreadSafeQueue<ValueType>&       queue )
{
    while( std::chrono::system_clock::now() < end_at )
    {
        auto loop_start_time = std::chrono::system_clock::now();
        auto loop_end_time   = loop_start_time + sleep_time;

        // Create new instance
        auto new_value = ValueType::create();

        // Add a value to the queue
        if( !queue.try_pop( new_value ) ) {

            // Sleep for the remaining time
            std::this_thread::sleep_until( loop_end_time );

        } else {

            // Sleep a trivial amount and try again
            std::this_thread::sleep_for( std::chrono::milliseconds(5) );
        }

    }
}

/**
 * Primary Destruction Thread
 */
template <typename ValueType>
void destroy_objects( std::chrono::system_clock::time_point  end_at,
                      std::chrono::milliseconds              sleep_time,
                      mem::ThreadSafeQueue<ValueType>&       queue )
{
    ValueType new_value;

    while( std::chrono::system_clock::now() < end_at )
    {
        auto loop_start_time = std::chrono::system_clock::now();
        auto loop_end_time   = loop_start_time + sleep_time;

        // Try to fetch a value.
        if( !queue.try_pop( new_value ) ) {

            // Sleep for the remaining time
            std::this_thread::sleep_until( loop_end_time );

        } else {

            // Sleep a trivial amount and try again
            std::this_thread::sleep_for( std::chrono::milliseconds(5) );
        }
    }
}

int main( int argc, char* argv[] )
{
    /**
     * This demo works by creating instances of a class on a different thread. 
     * It then destructs instances of each class on a different thread, at a different rate
     * as well. 
     */
    std::vector<std::thread> threads;

    // Resolve how long we need to run
    auto start_time = std::chrono::system_clock::now();
    auto run_time   = std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::seconds( 5 ) );
    auto end_time   = start_time + run_time;

    // List of queues holding data
    mem::ThreadSafeQueue<Class1> class1_queue;
    mem::ThreadSafeQueue<Class2> class2_queue;

    // Creation Rate Times
    std::chrono::milliseconds class1_create_sleep_time { 100 };
    std::chrono::milliseconds class2_create_sleep_time { 50 };

    // Destruction Rate Time
    //     Note:  If destroy > create, then you should expect a memory-leak like condition
    std::chrono::milliseconds class1_destroy_sleep_time { 100 };
    std::chrono::milliseconds class2_destroy_sleep_time { 50 };


    // Launch creation threads
    threads.emplace_back( &create_objects<Class1>, end_time, class1_create_sleep_time, std::ref( class1_queue ) );
    threads.emplace_back( &create_objects<Class2>, end_time, class2_create_sleep_time, std::ref( class2_queue ) );

    // Launch destruction threads
    threads.emplace_back( &destroy_objects<Class1>, end_time, class1_destroy_sleep_time, std::ref( class1_queue ) );
    threads.emplace_back( &destroy_objects<Class2>, end_time, class2_destroy_sleep_time, std::ref( class2_queue ) );


    return 0;
}
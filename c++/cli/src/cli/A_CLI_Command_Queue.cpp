/**
 * @file    A_CLI_Command_Queue.cpp
 * @author  Marvin Smith
 * @date    5/19/2015
*/
#include "A_CLI_Command_Queue.hpp"

namespace CLI{


/******************************/
/*        Constructor         */
/******************************/
A_CLI_Command_Queue::A_CLI_Command_Queue( const int& max_queue_size )
  : m_command_queue(new CMD::A_CLI_Command_Result[max_queue_size] ),
    m_head(0),
    m_tail(0),
    m_max_queue_size(max_queue_size),
    m_close_flag(false)
{
    // Initialize the semaphores
    sem_init( &m_push_semaphore, 0, max_queue_size);
    sem_init( &m_pop_semaphore, 0, 0);
}


/*********************************/
/*          Destructor           */
/*********************************/
A_CLI_Command_Queue::~A_CLI_Command_Queue()
{
    // Delete the queue
    delete [] m_command_queue;

    // Set to null
    m_command_queue = nullptr;
}


/**************************/
/*         Clear          */
/**************************/
void A_CLI_Command_Queue::Clear()
{
    // Set close flag
    m_close_flag = true;

    // Increment all semaphores
    sem_post( &m_push_semaphore );
    sem_post( &m_pop_semaphore );

}


/*****************************/
/*        Push Command       */
/*****************************/
void A_CLI_Command_Queue::Push_Command( CMD::A_CLI_Command_Result const& command )
{
    // Decrement the push semaphore
    sem_wait( &m_push_semaphore );

    // Lock the mutex
    m_mtx.lock();

    // Set the value
    m_command_queue[m_head] = command;

    // Increment the head
    m_head = (m_head+1) % m_max_queue_size;

    // Release the Mutex
    m_mtx.unlock();

    // Increment the pop semaphore
    sem_post( &m_pop_semaphore );
}

/*****************************/
/*        Pop Command        */
/*****************************/
CMD::A_CLI_Command_Result A_CLI_Command_Queue::Pop_Command()
{
    // Output
    CMD::A_CLI_Command_Result command;

    // Decrement the pop semaphore
    sem_wait( &m_pop_semaphore );
    
    // Return if the list is empty
    if( m_close_flag == true ){
        return CMD::A_CLI_Command_Result();
    }

    // Lock the mutex
    m_mtx.lock();
    
    // Get the value
    command = m_command_queue[m_tail];

    // Update the tail
    m_tail = (m_tail+1) % m_max_queue_size;
    
    // Unlock the mutex
    m_mtx.unlock();

    // Increment the push semaphore
    sem_post( &m_push_semaphore );
    
    // return command
    return command;
}

} // End of CLI Namespace


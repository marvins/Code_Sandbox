/**
 * @file    A_CLI_Command_Queue.hpp
 * @author  Marvin Smith
 * @date    5/19/2015
*/
#ifndef __CLI_A_CLI_COMMAND_QUEUE_HPP__
#define __CLI_A_CLI_COMMAND_QUEUE_HPP__

// C++ Standard Libraries
#include <atomic>
#include <memory>
#include <mutex>
#include <semaphore.h>

// CLI Libraries
#include "cmd/A_CLI_Command_Result.hpp"

namespace CLI{

/**
 * @class A_CLI_Command_Queue
*/
class A_CLI_Command_Queue{

    public:
        
        /// Pointer Type
        typedef std::shared_ptr<A_CLI_Command_Queue> ptr_t;


        /**
         * @brief Constructor
        */
        A_CLI_Command_Queue( const int& max_queue_size );
        

        /**
         * @brief Destructor
        */
        ~A_CLI_Command_Queue();

        
        /**
         * @brief Add Command
         */
        void Push_Command( CMD::A_CLI_Command_Result const& command );


        /**
         * @brief Pop Command
         */
        CMD::A_CLI_Command_Result Pop_Command();


        /**
         * @brief Clear the Semaphore
        */
        void Clear();


    private:
        
        /// Command Queue
        CMD::A_CLI_Command_Result* m_command_queue;
    
        /// Main Mutex
        std::mutex m_mtx;

        /// Counters
        sem_t m_push_semaphore;
        sem_t m_pop_semaphore;

        /// Head and Tail Cursors
        int m_head;
        int m_tail;

        /// Max Queue Size
        int m_max_queue_size;

        /// Close Flag
        std::atomic<bool> m_close_flag;

}; // End of A_CLI_Command_Queue Class

} // End of CLI Namespace

#endif

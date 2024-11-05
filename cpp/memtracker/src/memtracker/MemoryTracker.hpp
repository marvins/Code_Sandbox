/**
 * @file    MemoryTracker.h
 * @author  Marvin Smith
 * @date    11/3/2024
 */
#pragma once

// Boost Libraries
#include <boost/lockfree/queue.hpp>

// C++ Standard Libraries
#include <chrono>
#include <memory>

// Project Libraries
#include <memtracker/Result.hpp>

namespace mem {

// How we track unique classes
using Class_Unique_ID_TP = int32_t;

// How we track unique events
using Instance_Unique_ID_TP = int64_t;

// We want to track Memory Usage
using Memory_TP = int64_t;

enum class MemoryAction : int64_t {
    CREATE  = 0,
    DESTROY = 1,
}

// Each thing we want to track is 
using EventType = std::tuple<Instance_Unique_ID_TP,MemoryAction,Memory_TP>;

class MemoryReporter {

    public:

        MemoryReporter() = default;

        void report_create( EventType event ) {
            m_queue.push( event );
        }

        void update() {

            // Pop all entries off the queue
            while( !m_queue.empty() ){

                // Pop the next entry off

            }
        }

    private:

        // Storing unprocessed events
        boost::lockfree::queue<EventType> m_queue;

        // List of created events
        std::map<Instance_Unique_ID_TP,Memory_TP> m_created_queue;


}; // End of MemoryReporter class

/**
 * @class MemoryTracker
 */
template <typename CLASS_UID_TP = Class_Unique_ID_TP,
          typename INST_UID_TP  = Instance_Unique_ID_TP>
class MemoryTracker {

    public:

        using THIS_TP = MemoryTracker<CLASS_UID_TP,INST_UID_TP>;

        /**
         * Report Creation
         * @param id  Unique Identifier for the memory-tracker.  If a 
         */
        Result<void,StatusType> report_creation( CLASS_UID_TP  id,
                                                 INST_UID_TP   instance_id,
                                                 int64_t       allocation_size_bytes );

        /**
         * @brief Initialize the Memory Tracker
         * 
         * @param log_interval Amount of time between log reporting
         */
        static Result<void,StatusType> initialize( std::chrono::milliseconds log_interval );
        
        /**
         * @brief Get an instance of the MemoryTracker class.
         */
        static Result<THIS_TP&,StatusType> get_instance() {
            if( g_inst ){
                return Result<THIS_TP&,StatusType>( *g_inst );
            } else {
                return Result<THIS_TP&,StatusType>(Err<StatusType>());
            }
        }

    private:

        /// Global Instance
        static std::unique_ptr<MemoryTracker<CLASS_UID_TP,INST_UID_TP>> g_inst;

        /// Internal Memory Loggers
        std::map<CLASS_UID_TP,MemoryReporter<EventType>> m_reporters;


}; // End of MemoryTracker class

} // End of mem namespace
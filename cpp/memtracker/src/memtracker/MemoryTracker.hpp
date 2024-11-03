/**
 * @file    MemoryTracker.h
 * @author  Marvin Smith
 * @date    11/3/2024
 */
#pragma once

// Boost Libraries
#include <boost/lockfree/queue.hpp>

// C++ Standard Libraries
#include <memory>

// Project Libraries
#include <memtracker/Result.hpp>

namespace mem {

class MemoryReporter {

    public:

    private:



}; // End of MemoryReporter class

/**
 * @class MemoryTracker
 */
template <typename CLASS_UID_TP = int32_t,
          typename INST_UID_TP  = int64_t>
class MemoryTracker {

    public:

        /**
         * Report Creation
         * @param id  Unique Identifier for the memory-tracker.  If a 
         */
        Result<void> report_creation( CLASS_UID_TP  id,
                                      INST_UID_TP   instance_id,
                                      int64_t       allocation_size_bytes );

        /**
         * @brief Initialize the Memory Tracker
         * 
         * @param log_interval Amount of time between log reporting
         */
        static Result<void> initialize( std::chrono::milliseconds log_interval );
        
        /**
         * @brief Get an instance of the MemoryTracker class.
         */
        static Result<MemoryTracker&> get_instance() {
            if( g_inst ){
                return Result<MemoryTracker&>( *g_inst );
            } else {
                return Result(Ok());
            }
        }

    private:

        /// Global Instance
        static std::unique_ptr<MemoryTracker<CLASS_UID_TP,INST_UID_TP>> g_inst;


}; // End of MemoryTracker class

} // End of mem namespace
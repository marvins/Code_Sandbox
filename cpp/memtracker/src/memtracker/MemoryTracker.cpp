/**
 * @file    MemoryTracker.cpp
 * @author  Marvin Smith
 * @date    11/3/2024
 */
#include <memtracker/MemoryTracker.hpp>

// C++ Standard Libraries
#include <memory>

namespace mem {

// Global copy of tracker
static std::unique_ptr<MemoryTracker> g_instance;

/************************************************/
/*          Grab the global instance            */
/************************************************/
MemoryTracker& MemoryTracker::get_instance()
{

}

} // End of mem namespace
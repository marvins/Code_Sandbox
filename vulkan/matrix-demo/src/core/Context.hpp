/**
 * @file    Context.hpp
 * @author  Marvin Smith
 * @date    12/14/2024
 */
#pragma once

// Vulkan Libraries
#include <vulkan/vulkan.h>

// C++ Libraries
#include <string>

class Context {

    public:

        /**
         * Config struct
        */
        struct Config {
            
            std::string app_name;

        }; // End of Config struct

        Context();

    protected:

        void create_instance();

    private:

    


}; // End of Context class
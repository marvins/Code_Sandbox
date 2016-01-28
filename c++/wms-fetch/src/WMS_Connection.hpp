/**
 * @file    WMS_Connection.hpp
 * @author  Marvin Smith
 * @date    1/27/2015
*/
#ifndef __WMS_CONNECTION_HPP__
#define __WMS_CONNECTION_HPP__


// C++ Libraries
#include <memory>
#include <string>


/**
 * @class WMS_Connection
*/
class WMS_Connection
{
    public:
        
        /// Pointer Type
        typedef std::shared_ptr<WMS_Connection> ptr_t;
        

        /**
         * @brief Constructor
        */
        WMS_Connection();


        /**
         * @brief Connect to the server.
        */
        void Connect();


        /**
         * @brief Get the Capabilities.
        */
        void Get_Capabilities();


        /**
         * @brief Get the Map
        */
        void Get_Map();


    private:
        
        /// Class Name
        std::string m_class_name;


}; // End of WMS_Connection Class


#endif

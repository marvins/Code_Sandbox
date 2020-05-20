#pragma once

// C++ Libraries
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <vector>

/**
 * @brief Resource-Management utility class.
 * @tparam ResourceType
 */
template <typename ResourceType>
class Resource_Manager
{
    public:

        /**
         * @brief Parameterized Constructor
         * @param max_resources
         * @param default_allocator
         */
        explicit Resource_Manager( size_t                        max_resources = 10,
                                   std::function<ResourceType()> default_allocator = []{ return ResourceType(); } )
          : m_default_allocator( default_allocator )
        {
            for( size_t i=0; i<max_resources; i++ )
            {
                m_resource_list.push_back( m_default_allocator );
            }
        }

        /**
         * @brief Destructor
         */
        virtual ~Resource_Manager() = default;

        /**
         * @brief Return the number of resources available.
         * @return
         */
        size_t Get_Number_Resources() const
        {
            return m_resource_list.size();
        }

        /**
         * @brief Get the next available resource
         * @return
         */
        std::shared_ptr<ResourceType> Get_Resource( bool& valid )
        {
            std::lock_guard<std::mutex> lck( m_resource_mtx );
            valid = false;

            // Iterate over each resource, looking for the next available one
            int counter = 0;
            for( size_t idx = 0; idx < m_resource_list.size(); idx++ )
            {
                if( !m_resource_list[idx].m_reserved )
                {
                    valid = true;
                    m_resource_list[idx].m_reserved = true;
                    return m_resource_list[idx].m_resource;
                }
                counter++;
            }
            return nullptr;
        }

        /**
         * @brief Release a single resource back to the manager.
         * @param resource
         */
        void Release( std::shared_ptr<ResourceType>& resource )
        {
            std::lock_guard<std::mutex> lck( m_resource_mtx );
            for( size_t idx = 0; idx < m_resource_list.size(); idx++ )
            {
                if( m_resource_list[idx].m_resource == resource )
                {
                    m_resource_list[idx].m_reserved = false;
                    return;
                }
            }

            // Clear the input pointer
            resource = nullptr;
        }

        std::string To_Log_String()
        {
            std::ostringstream sout;
            sout << "Resource-Manager" << std::endl;
            for( size_t i=0; i<m_resource_list.size(); i++ )
            {
                sout << "  - " << i << ", Reserved: " << std::boolalpha << m_resource_list[i].m_reserved;
                sout << ", Ptr: " << m_resource_list[i].m_resource << ", Contents: ";
                sout << (*m_resource_list[i].m_resource) << std::endl;
            }
            return sout.str();
        }

    private:

        struct ResourcePair
        {
            /**
             * @brief Default Constructor
             */
            ResourcePair( std::function<ResourceType()> resource_allocator )
              : m_resource( std::make_shared<ResourceType>( resource_allocator() ) ),
                m_reserved( false )
            {}

            std::shared_ptr<ResourceType> m_resource;
            bool m_reserved { false };
        };

        /// Method to build default units
        std::function<ResourceType()> m_default_allocator;

        /// List of resources
        std::vector<ResourcePair> m_resource_list;

        /// Mutex
        std::mutex m_resource_mtx;
};
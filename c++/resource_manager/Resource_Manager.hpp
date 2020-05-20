#pragma once

// C++ Libraries
#include <functional>
#include <memory>
#include <mutex>
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
            m_resource_list.resize( max_resources, m_default_allocator );
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
            for( auto& resource : m_resource_list )
            {
                if( !resource.m_reserved )
                {
                    valid = true;
                    resource.m_reserved = true;
                    return resource.m_resource;
                }
            }
            return nullptr;
        }

        void Release( std::unique_ptr<ResourceType> resource )
        {

        }

    private:

        struct ResourceTuple
        {
            /**
             * @brief Default Constructor
             */
            ResourceTuple( std::function<ResourceType()> resource_allocator )
              : m_resource( std::make_shared<ResourceType>(resource_allocator()) ),
                m_reserved( false )
            {}

            std::shared_ptr<ResourceType> m_resource;
            bool m_reserved { false };
        };

        /// Method to build default units
        std::function<ResourceType()> m_default_allocator;

        /// List of resources
        std::vector<ResourceTuple> m_resource_list;

        /// Mutex
        std::mutex m_resource_mtx;
};
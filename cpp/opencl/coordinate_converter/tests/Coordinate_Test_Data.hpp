/**
 * @file    Coordinate_Test_Data.hpp
 * @author  Marvin Smith
 * @date    1/13/2017
 */
#ifndef LIB_COORDINATE_TEST_COORDINATE_TEST_DATA_HPP
#define LIB_COORDINATE_TEST_COORDINATE_TEST_DATA_HPP

// C++ Libraries
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

// Project Libraries
#include <Coordinate.hpp>


/**
 * @struct Coordinate_Tuple
 */
struct Coordinate_Tuple
{
    public:

        /**
         * @brief Default Constructor
         */
        Coordinate_Tuple();

        /**
         * @brief Constructor
         * @param location_name
         */
        Coordinate_Tuple( const std::string& location_name );


        /**
         * @brief Get the Location Name
         * @return
         */
        inline std::string Get_Location_Name()const{
            return m_location_name;
        }


        /**
         * @brief Add Coordinate
         */
        inline void Add_Coordinate( const Coordinate_Base::ptr_t new_coord ){
            m_coordinates.push_back(new_coord);
        }


        /**
         * @brief Return a specific coordinate
         */
        Coordinate_Base::ptr_t  Get_Coordinate( const CoordinateType&  coordinate_type,
                                                const DatumType&       datum_type );


        /**
         * @brief Comparison Operator
         * @param rhs
         * @return
         */
        inline bool operator < ( const Coordinate_Tuple& rhs )const{
            return m_location_name < rhs.m_location_name;
        }


    private:

        /// Coordinate-Name
        std::string m_location_name;

        /// Set of Coordinates
        std::vector<Coordinate_Base::ptr_t> m_coordinates;

};

/**
 * @brief Coordinate Reference Information
 */
class Coordinate_Test_Data
{
    public:

        /**
         * @brief Parse the Coordinate Test Data
         * @param reference_file
         */
        static void Initialize( const std::string& reference_file );

        /**
         * @brief Cleanup the Coordinate Test Data
         */
        static void Finalize();


        /**
         * @brief Return a list of Coordinates
         */
        static std::map<std::string,Coordinate_Base::ptr_t>  Get_Coordinates( const CoordinateType&  coordinate_type,
                                                                              const DatumType&       datum_type );

    private:

        /**
         * @brief Constructor
         */
        Coordinate_Test_Data();


        /// List of Locations
        std::map<std::string,Coordinate_Tuple> m_coordinate_list;
};


#endif
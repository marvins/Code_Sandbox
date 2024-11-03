/**
 * @file    Coordinate_Test_Data.cpp
 * @author  Marvin Smith
 * @date    1/13/2017
 */
#include "Coordinate_Test_Data.hpp"

// Pugi
#include <pugixml.hpp>

// C++ Libraries
#include <iostream>

/// Local Reference
Coordinate_Test_Data*  instance = nullptr;


/********************************/
/*          Constructor         */
/********************************/
Coordinate_Tuple::Coordinate_Tuple()
  : m_location_name("")
{
}

/********************************/
/*          Constructor         */
/********************************/
Coordinate_Tuple::Coordinate_Tuple(const std::string &location_name)
  : m_location_name(location_name)
{
}


/************************************************/
/*          Get the Requested Coordinate        */
/************************************************/
Coordinate_Base::ptr_t  Coordinate_Tuple::Get_Coordinate(const CoordinateType& coordinate_type,
                                                         const DatumType&      datum_type )
{
    // iterate over each coordinate
    for( auto coord : m_coordinates )
    {
        // Make sure not null
        if( coord == nullptr ){

        }

        // Check if coordinate type and datum type match
        else if( coord->Get_Datum() == datum_type &&
                 coord->Get_Type()  == coordinate_type )
        {
            return coord;
        }
    }
    return nullptr;
}


/***********************************************/
/*          Initialize the Test Data           */
/***********************************************/
void Coordinate_Test_Data::Initialize( const std::string& reference_file)
{
    // Create instance
    if( instance == nullptr ){
        instance = new Coordinate_Test_Data();
    }

    // Open PugiXML Document
    pugi::xml_document xml_doc;
    pugi::xml_parse_result res = xml_doc.load_file( reference_file.c_str() );

    // If problems, throw error
    if( !res )
    {
        throw std::runtime_error("Unable to load reference file at: " + reference_file);
    }

    // Otherwise, grab the root node
    pugi::xml_node root_node = xml_doc.child("Coordinate_Conversion");
    std::cout << "Root Node: " << root_node.name() << std::endl;

    // Grab the coordinate node
    pugi::xml_node coordinates_node = root_node.child("Coordinates");

    if( !coordinates_node ){
        throw std::runtime_error("Unable to find <Coordinates> node.");
    }

    for( auto node = coordinates_node.begin(); node != coordinates_node.end(); node++ )
    {
        // Grab the node
        pugi::xml_node coord_node = (*node);

        // Get the name
        std::string location_name = coord_node.attribute("name").as_string();

        // Add the node
        instance->m_coordinate_list[location_name] = Coordinate_Tuple(location_name);

        //Iterate over subnodes
        for( auto subnode = coord_node.begin(); subnode != coord_node.end(); subnode++ )
        {
            // Check the type
            if( subnode->name() == std::string("Geog"))
            {
                // Get the Datum
                std::string datum_str = subnode->attribute("datum").as_string();
                DatumType datum = String_To_DatumType(datum_str);

                // Latitude and Longitude
                double lat_deg = subnode->attribute("latitude_deg").as_double();
                double lon_deg = subnode->attribute("longitude_deg").as_double();

                // Add to coordinate list
                instance->m_coordinate_list[location_name].Add_Coordinate( std::make_shared<Coordinate_Geographic>( lon_deg,
                                                                                                                    lat_deg,
                                                                                                                    datum ));

            }
            else if( subnode->name() == std::string("UTM"))
            {

                // Get the Grid Zone
                int grid_zone = subnode->attribute("grid_zone").as_int();

                // Get the hemisphere
                bool is_north = subnode->attribute("is_northern").as_bool();

                // Get the Easting
                double easting_m  = subnode->attribute("easting_meters").as_double();
                double northing_m = subnode->attribute("northing_meters").as_double();

                // Add to coordinate list
                instance->m_coordinate_list[location_name].Add_Coordinate( std::make_shared<Coordinate_UTM>( grid_zone,
                                                                                                             is_north,
                                                                                                             easting_m,
                                                                                                             northing_m ));

            }
            else if( subnode->name() == std::string("MGRS"))
            {

            }
            else
            {
                throw std::runtime_error("Unknown Node (" + std::string(subnode->name()) + ")");
            }

        }
    }

}


/********************************************/
/*          Finalize the Test Data          */
/********************************************/
void Coordinate_Test_Data::Finalize()
{
    if( instance != nullptr ){
        delete instance;
        instance = nullptr;
    }
}


/**************************************************/
/*          Get the Specified Coordinate          */
/**************************************************/
std::map<std::string,Coordinate_Base::ptr_t>  Coordinate_Test_Data::Get_Coordinates(  const CoordinateType& coordinate_type,
                                                                                      const DatumType&      datum_type)
{
    // Return type
    std::map<std::string,Coordinate_Base::ptr_t>  output;


    // Iterate over each tuple
    for( auto tup : instance->m_coordinate_list )
    {
        // Query for the coordinate
        auto coord = tup.second.Get_Coordinate( coordinate_type,
                                                datum_type );

        if( coord != nullptr ){
            output[tup.first] = coord;
        }
    }

    return output;
}


/********************************/
/*          Constructor         */
/********************************/
Coordinate_Test_Data::Coordinate_Test_Data()
{
}

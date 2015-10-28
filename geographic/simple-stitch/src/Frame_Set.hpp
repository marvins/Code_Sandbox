/**
 * @file    Frame_Set.hpp
 * @author  Marvin Smith
 * @date    10/27/2015
*/
#ifndef __FRAME_SET_HPP__
#define __FRAME_SET_HPP__


// C++ Libraries
#include <map>
#include <string>
#include <vector>

// Options
#include "Options.hpp"


/**
 * @class Frame_Set
*/
class Frame_Set
{
    public:
        
        /**
         * @brief Constructor
         *
         * @param[in] frame_number
         */
         Frame_Set( const int& frame_set );


         /**
          * @brief Get the Frame Number.
          *
          * @return Frame number.
         */
         inline int Get_Frame_Number()const{
             return m_frame_number;
         }


         /**
          * @brief Get the Tile List.
          *
          * @return Get the tile list.
         */
         inline std::map<int,std::string> Get_Tile_List()const{
             return m_tile_list;
         }
         

     private:
        
        /// Class Name
        std::string m_class_name;

        /// Frame Number
        int m_frame_number;

        /// List of Tiles
        std::map<int,std::string> m_tile_data;


}; // End of Frame_Set Class


/**
 * @brief Load Frame Sets
 *
 * @param[in] options
 * @param[out] frame_sets
*/
void Load_Frame_Sets( const Options&            options,
                      std::map<int,Frame_Set>&  frame_sets );


#endif

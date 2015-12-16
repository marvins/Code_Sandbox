#include "J2K_Parsers.hpp"

// C++ Libraries
#include <cstring>
#include <iostream>


/// Markers
std::vector<J2K_Marker_Base::ptr_t> marker_list;

/**********************************/
/*         J2K Marker Base        */
/**********************************/
J2K_Marker_Base::J2K_Marker_Base()
{
}


/******************************/
/*      J2K SOC Contains      */
/******************************/
bool J2K_Marker_Base::Contains( const uint8_t* buffer )const
{
    // check if buffer is null
    if( memcmp( buffer, m_marker_tag.c_str(), m_marker_tag.size() ) == 0 )
    {
        return true;
    }

    return false;
}


/*****************************/
/*       J2K SOC Marker      */
/*****************************/
J2K_SOC_Marker::J2K_SOC_Marker()
{
    m_marker_tag.resize(2);
    m_marker_tag[0] = 0xff;
    m_marker_tag[1] = 0x4f;
}


/**************************************/
/*         J2K SOC Process            */
/**************************************/
void J2K_SOC_Marker::Process( const uint8_t* buffer,
                              int64_t&       index )
{
    // Log Entry 
    std::cout << std::dec << index << ", SOC - Start of Codestream" << std::endl;
    
    // Increment by 2
    index += m_marker_tag.size();

}

/*****************************/
/*       J2K SOD Marker      */
/*****************************/
J2K_SOD_Marker::J2K_SOD_Marker()
{
    m_marker_tag.resize(2);
    m_marker_tag[0] = 0xff;
    m_marker_tag[1] = 0x93;
}


/**************************************/
/*         J2K SOC Process            */
/**************************************/
void J2K_SOD_Marker::Process( const uint8_t* buffer,
                              int64_t&       index )
{
    // Log Entry 
    std::cout << std::dec << index << ", SOD - Start of Data" << std::endl;
    
    // Increment by 2
    index += m_marker_tag.size();

}



/*****************************/
/*       J2K SOT Marker      */
/*****************************/
J2K_SOT_Marker::J2K_SOT_Marker()
{
    m_marker_tag.resize(2);
    m_marker_tag[0] = 0xff;
    m_marker_tag[1] = 0x90;
}


/**************************************/
/*         J2K SOT Process            */
/**************************************/
void J2K_SOT_Marker::Process( const uint8_t* buffer,
                              int64_t&       index )
{
    // Log Entry 
    std::cout << std::dec << index << ", SOT - Start of Tile" << std::endl;
    
    // Increment by 2
    index += m_marker_tag.size();

}

/*****************************/
/*       J2K SIZ Marker      */
/*****************************/
J2K_SIZ_Marker::J2K_SIZ_Marker()
{
    m_marker_tag.resize(2);
    m_marker_tag[0] = 0xff;
    m_marker_tag[1] = 0x51;
}


/**************************************/
/*         J2K SOT Process            */
/**************************************/
void J2K_SIZ_Marker::Process( const uint8_t* buffer,
                              int64_t&       index )
{
    // Log Entry 
    std::cout << std::dec << index << ", SIZ - Image and Tile Size" << std::endl;
    
    // Increment by 2
    index += m_marker_tag.size();
    
    // LSize (Length of Marker Segment
    short Lsiz = reinterpret_cast<const short*>(buffer + index)[0];
    index += 2;
    std::cout << std::dec << "  Lsiz: " << Lsiz << std::endl;

    // Rsiz
    short Rsiz = reinterpret_cast<const short*>(buffer + index)[0];
    index += 2;
    std::cout << std::dec << "  Rsiz: " << Rsiz << std::endl;

    int   Xsiz;
    int   Ysiz;
    int   XOsiz;
    int   XTsiz;
    int   YTsiz;
}


/*****************************/
/*       J2K COC Marker      */
/*****************************/
J2K_COC_Marker::J2K_COC_Marker()
{
    m_marker_tag.resize(2);
    m_marker_tag[0] = 0xff;
    m_marker_tag[1] = 0x53;
}


/**************************************/
/*         J2K COC Process            */
/**************************************/
void J2K_COC_Marker::Process( const uint8_t* buffer,
                              int64_t&       index )
{
    // Log Entry 
    std::cout << std::dec << index << ", COC - Coding Style Component" << std::endl;
    
    // Increment by 2
    index += m_marker_tag.size();

}

/*****************************/
/*       J2K SOC Marker      */
/*****************************/
J2K_EOC_Marker::J2K_EOC_Marker()
{
    m_marker_tag.resize(2);
    m_marker_tag[0] = 0xff;
    m_marker_tag[1] = 0xd9;
}


/**************************************/
/*         J2K EOC Process            */
/**************************************/
void J2K_EOC_Marker::Process( const uint8_t* buffer,
                              int64_t&       index )
{
    // Log Entry 
    std::cout << index << ", EOC - End of Codestream" << std::endl;
    
    // Increment by 2
    index += m_marker_tag.size();

}


/*****************************************/
/*          J2K Marker Container         */
/*****************************************/
J2K_Marker_Container::J2K_Marker_Container()
{
    // Add Containers
    marker_list.push_back(std::make_shared<J2K_SOC_Marker>());
    marker_list.push_back(std::make_shared<J2K_SOD_Marker>());
    marker_list.push_back(std::make_shared<J2K_SOT_Marker>());
    marker_list.push_back(std::make_shared<J2K_SIZ_Marker>());
    marker_list.push_back(std::make_shared<J2K_COC_Marker>());
    marker_list.push_back(std::make_shared<J2K_EOC_Marker>());

}


/****************************************/
/*           Process Markers            */
/****************************************/
void J2K_Marker_Container::Process_Markers( const uint8_t* buffer,
                                            int64_t&       index,
                                            const int64_t& max_size  )
{
    // Misc Flags
    bool marker_found;

    std::cout << std::dec << "Processing Markers" << std::endl;

    // Iterate over the buffer
    while( index < max_size )
    {
        // Set the found flag
        marker_found = false;

        // Iterate over markers
        for( size_t i=0; i<marker_list.size(); i++ )
        {
            // Check if there is a match
            if( marker_list[i]->Contains( buffer + index ) == true )
            {
                marker_list[i]->Process( buffer,
                                         index );
                marker_found = true;
                break;
            }
        }

        // If not found, print
        if( marker_found == false )
        {
            std::cout << std::dec << index << ", No Marker Found. Data: " << std::hex << (int)buffer[index] << std::dec << std::endl;
            index++;
        }
    }

}



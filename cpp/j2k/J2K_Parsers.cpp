#include "J2K_Parsers.hpp"

// C++ Libraries
#include <cstring>
#include <iostream>
#include <netinet/in.h>


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
    index += m_marker_tag.size();

    // Lsot - Length of Marker Segment
    uint16_t Lsot = ntohs(reinterpret_cast<const uint16_t*>(buffer + index)[0]);
    index += 2;
    std::cout << std::dec << "   - Lsot: " << Lsot << std::endl;

    // Isot - Tile Index
    uint16_t Isot = ntohs(reinterpret_cast<const uint16_t*>(buffer + index)[0]);
    index += 2;
    std::cout << std::dec << "   - Isot: " << Isot << std::endl;

    // Psot
    uint32_t Psot = ntohl(reinterpret_cast<const uint32_t*>(buffer + index)[0]);
    index += 4;
    std::cout << "   - Psot: " << Psot << std::endl;

    // Tile Part Index
    uint8_t TPsot = buffer[index];
    index += 1;
    std::cout << "   - TPsot: " << (int)TPsot << std::endl;


    // Tile Number Parts
    uint8_t TNsot = buffer[index];
    index += 1;
    std::cout << "   - TNsot: " << (int)TNsot << std::endl;    
    

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
    uint16_t Lsiz = ntohs(reinterpret_cast<const uint16_t*>(buffer + index)[0]);
    index += 2;
    std::cout << std::dec << "    - Lsiz: " << Lsiz << std::endl;

    // Rsiz
    uint16_t Rsiz = ntohs(reinterpret_cast<const uint16_t*>(buffer + index)[0]);
    index += 2;
    std::cout << std::dec << "    - Rsiz: " << Rsiz << std::endl;

    // Xsiz
    uint32_t   Xsiz = ntohl(reinterpret_cast<const uint32_t*>(buffer + index)[0]);
    index += 4;
    std::cout << "    - Xsiz: " << Xsiz << std::endl;

    // Ysiz
    uint32_t   Ysiz = ntohl(reinterpret_cast<const uint32_t*>(buffer + index)[0]);
    index += 4;
    std::cout << "    - Ysiz: " << Ysiz << std::endl;
    
    // XOsiz
    uint32_t  XOsiz = ntohl(reinterpret_cast<const uint32_t*>(buffer + index)[0]);
    index += 4;
    std::cout << "    - XOsiz: " << XOsiz << std::endl;
    
    // YOsiz
    uint32_t  YOsiz = ntohl(reinterpret_cast<const uint32_t*>(buffer + index)[0]);
    index += 4;
    std::cout << "    - YOsiz: " << YOsiz << std::endl;
    
    // XTsiz
    uint32_t  XTsiz = ntohl(reinterpret_cast<const uint32_t*>(buffer + index)[0]);
    index += 4;
    std::cout << "    - XTsiz: " << XTsiz << std::endl;
    
    // YTsiz
    uint32_t  YTsiz = ntohl(reinterpret_cast<const uint32_t*>(buffer + index)[0]);
    index += 4;
    std::cout << "    - YTsiz: " << YTsiz << std::endl;
    
    // XTOsiz
    uint32_t  XTOsiz = ntohl(reinterpret_cast<const uint32_t*>(buffer + index)[0]);
    index += 4;
    std::cout << "    - XTOsiz: " << XTOsiz << std::endl;
    
    // YTOsiz
    uint32_t  YTOsiz = ntohl(reinterpret_cast<const uint32_t*>(buffer + index)[0]);
    index += 4;
    std::cout << "    - YTOsiz: " << YTOsiz << std::endl;
    
    // Csiz
    uint16_t  Csiz   = ntohs(reinterpret_cast<const uint16_t*>(buffer + index)[0]);
    index += 2;
    std::cout << "    - Csiz: " << Csiz << std::endl;

    for( int i=0; i<Csiz; i++ )
    {
        // Ssiz
        uint8_t Ssiz  = buffer[index];
        index += 1;

        // XRsiz
        uint8_t XRsiz = buffer[index];
        index += 1;

        // YRsiz
        uint8_t YRsiz = buffer[index];
        index += 1;
        std::cout << "    - Component " << i << std::endl;
        std::cout << "       - Ssiz: " << (int)Ssiz << std::endl;
        std::cout << "       - XRsiz: " << (int)XRsiz << std::endl;
        std::cout << "       - YRsiz: " << (int)YRsiz << std::endl;
    } 
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
/*       J2K COD Marker      */
/*****************************/
J2K_COD_Marker::J2K_COD_Marker()
{
    m_marker_tag.resize(2);
    m_marker_tag[0] = 0xff;
    m_marker_tag[1] = 0x52;
}


/**************************************/
/*         J2K COD Process            */
/**************************************/
void J2K_COD_Marker::Process( const uint8_t* buffer,
                              int64_t&       index )
{
    // Log Entry 
    std::cout << std::dec << index << ", COD - Coding Style Default" << std::endl;
    index += m_marker_tag.size();
    
    // Lcod (Length of Marker Segment
    uint16_t Lcod = ntohs(reinterpret_cast<const uint16_t*>(buffer + index)[0]);
    index += 2;
    std::cout << "    - Lcod: " << Lcod << std::endl;


    // Scod (Coding Style)
    uint8_t Scod = buffer[index];
    index += 1;
    std::cout << "    - Scod: " << (int)Scod << std::endl;

    // SGcod
    uint32_t SGcod = ntohl(reinterpret_cast<const uint32_t*>(buffer+index)[0]);
    index += 4;
    std::cout << "    - SGcod: " << SGcod << std::endl;


    // SPcod
    std::cout << "    - SPcod: ";
    for( int len=Lcod-7; len>=0; len--)
    {
        // SPcod
        uint8_t SPcod = buffer[index];
        std::cout << std::hex << (short)SPcod;
    }
    std::cout << std::dec << std::endl;

}


/*****************************/
/*       J2K COM Marker      */
/*****************************/
J2K_COM_Marker::J2K_COM_Marker()
{
    m_marker_tag.resize(2);
    m_marker_tag[0] = 0xff;
    m_marker_tag[1] = 0x64;
}


/**************************************/
/*         J2K COM Process            */
/**************************************/
void J2K_COM_Marker::Process( const uint8_t* buffer,
                              int64_t&       index )
{
    // Log Entry 
    std::cout << std::dec << index << ", COM - Comment" << std::endl;
    index += m_marker_tag.size();
    
    // Lcom (Length of Marker Segment
    uint16_t Lcom = ntohs(reinterpret_cast<const uint16_t*>(buffer + index)[0]);
    index += 2;
    std::cout << "    - Lcom: " << Lcom << std::endl;


    // Rcom (Registration Value of Marker Segment
    uint16_t Rcom = ntohs(reinterpret_cast<const uint16_t*>(buffer + index)[0]);
    index += 2;
    std::cout << "    - Rcom: " << Rcom << std::endl;

    
    // Ccom Data
    std::cout << "    - Ccom: ";
    for( int len=Lcom - 5; len >= 0; len-- )
    {
        uint8_t Ccom = buffer[index];
        index += 1;
        std::cout << std::hex << Ccom;
    }
    std::cout << std::dec << std::endl;

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
    marker_list.push_back(std::make_shared<J2K_COC_Marker>());
    marker_list.push_back(std::make_shared<J2K_COD_Marker>());
    marker_list.push_back(std::make_shared<J2K_COM_Marker>());
    marker_list.push_back(std::make_shared<J2K_SOC_Marker>());
    marker_list.push_back(std::make_shared<J2K_SOD_Marker>());
    marker_list.push_back(std::make_shared<J2K_SOT_Marker>());
    marker_list.push_back(std::make_shared<J2K_SIZ_Marker>());
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



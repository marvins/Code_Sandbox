//
// Created by Marvin Smith on 1/1/18.
//

#include "Enumerations.hpp"


/****************************************************/
/*          Convert a DatumType to String           */
/****************************************************/
std::string DatumType_To_String( const DatumType& datum_type )
{
    if( datum_type == DatumType::WGS84 ){
        return "WGS84";
    }
    return "UNKNOWN";
}

/****************************************************/
/*          Convert a String to Datum Type          */
/****************************************************/
DatumType  String_To_DatumType( const std::string& datum_type )
{
    if( datum_type == "WGS84" || datum_type == "wgs84"){
        return DatumType::WGS84;
    }

    return DatumType::UNKNOWN;
}

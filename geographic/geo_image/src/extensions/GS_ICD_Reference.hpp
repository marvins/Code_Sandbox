#ifndef __GS_ICD_REFERENCE_HPP__
#define __GS_ICD_REFERENCE_HPP__

#include <string>


const int ACFTB_ICD_SIZE = 10;

std::string ACFTB_ICD_Reference[ACFTB_ICD_SIZE][4] = {
    {"AC_MSN_ID"         , "Aircraft Mission Identification"    , "20" , "BCS-A"}, //0
    {"AC_TAIL_NO"        , "Aircraft Tail Number"               , "10" , "BCS-A"}, //1
    {"AC_TO"             , "Aircraft Take Off"                  , "12" , "BCS-A"}, //2
    {"SENSOR_ID_TYPE"    , "Sensor ID Type"                     , "4"  , "BCS-A"}, //3
    {"SENSOR_ID"         , "Sensor ID"                          , "6"  , "BCS-A"}, //4
    {"SCENE_SOURCE"      , "Scene Source"                       , "1"  , "BCS-A"}, //5
    {"SCNUM"             , "Scene Number"                       , "6"  , "BCS-N"}, //6
    {"PDATE"             , "Processing Date"                    , "8"  , "BCS-N"}, //7
    {"IMHOSTNO"          , "Immediate Scene Host"               , "6"  , "BCS-N"}, //8
    {"IMREQID"           , "Immediate Scene Request ID"         , "5"  , "BCS-N"}, //9
    {"MPLAN"             , "Mission Plan Mode"                  , "3"  , "BCS-N"}, //10
    {"ENTLOC"            , "Entry Location"                     , "25" , "BCS-A"}, //11
    {"LOC_ACCY"          , "Location Accuracy"                  , "6"  , "BCS-N"}, //12
    {"ENTELV"            , "Entry Elevation"                    , "6"  , "BCS-A"}, //13
    {"ELV_UNIT"          , "Unit of Elevation"                  , "1"  , "BCS-A"}, //14
    {"EXITLOC"           , "Exit Location"                      , "25" , "BCS-A"}, //15
    {"EXITELV"           , "Exit Elevation"                     , "6"  , "BCS-A"}, //16
    {"TMAP"              , "True Map Angle"                     , "7"  , "BCS-A"}, //17
    {"ROW_SPACING"       , "Row Spacing"                        , "7"  , "BCS-A"}, //18
    {"ROW_SPACING_UNITS" , 
};



#endif

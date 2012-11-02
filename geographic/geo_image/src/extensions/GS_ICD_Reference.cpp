#ifndef __GS_ICD_REFERENCE_HPP__
#define __GS_ICD_REFERENCE_HPP__

#include <string>


const int ACFTB_ICD_SIZE  = 19;
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
    {"ROW_SPACING"       , "Row Spacing"                        , "7"  , "BCS-A"} //18
    //{"ROW_SPACING_UNITS" , 
};




const int AIMIDB_ICD_SIZE = 15;
std::string AIMIDB_ICD_Reference[AIMIDB_ICD_SIZE][4] = {
    {"ACQUISITION_DATE"       , "Acqusition Date and Time"           , "14" , "BCS-N"}, //0
    {"MISSION_NO"             , "Mission Number"                     , "4"  , "BCS-A"}, //1
    {"MISSION_IDENTIFICATION" , "Mission Identification"             , "10" , "BCS-A"}, //2
    {"FLIGHT_NO"              , "Flight Number"                      , "2"  , "BCS-A"}, //3
    {"OP_NUM"                 , "Image Operation Number"             , "3"  , "BCS-N"}, //4
    {"CURRENT_SEGMENT"        , "Current Segment ID"                 , "2"  , "BCS-A"}, //5
    {"REPRO_NUM"              , "Reprocess Number"                   , "2"  , "BCS-N"}, //6
    {"REPLAY"                 , "Replay"                             , "3"  , "BCS-N"}, //7
    {"RESERVED_001"           , "Reserved For Future Use"            , "1"  , "BCS-A"}, //8
    {"START_TILE_COLUMN"      , "Starting Tile Column Number"        , "3"  , "BCS-N"}, //9
    {"START_TILE_ROW"         , "Starting Tile Row Number"           , "5"  , "BCS-N"}, //10
    {"END_SEGMENT"            , "Ending Segment"                     , "2"  , "BCS-A"}, //11
    {"END_TILE_COLUMN"        , "Ending Tile Column"                 , "3"  , "BCS-N"}, //12
    {"END_TILE_ROW"           , "Ending Tile Row Number"             , "5"  , "BCS-N"}, //13
    {"COUNTRY"                , "Country Code"                       , "2"  , "BCS-A"} //14
};


const int BANDSB_ICD_SIZE = 21;
std::string BANDSB_ICD_Reference[BANDSB_ICD_SIZE][4] = {
    {"COUNT"                          , "Number of Bands"                    , "5"  , "BCS-N"  }, //0
    {"RADIOMETRIC_QUANTITY"           , "Data Representation"                , "24" , "BCS-A"  }, //1
    {"RADIOMETRIC_QUANTITY_UNIT"      , "Data Representation Unit"           , "1"  , "BCS-A"  }, //2
    {"SCALE_FACTOR"                   , "Cube Scale Factor"                  , "7"  , "IEEE32F"}, //3
    {"ADDITIVE_FACTOR"                , "Cube Additive Factor"               , "7"  , "IEEE32F"}, //4
    {"ROW_GSD"                        , "Row Ground Sample Distance"         , "7"  , "BCS-N"  }, //5
    {"ROW_GSD_UNIT"                   , "Units of Row Ground Sample Distance", "1"  , "BCS-A"  }, //6
    {"COL_GSD"                        , "Col Ground Sample Distance"         , "7"  , "BCS-N"  }, //7
    {"COL_GSD_UNIT"                   , "Units of Col Ground Sample Distance", "1"  , "BCS-A"  }, //8
    {"SPT_RESP_ROW"                   , "Spatial Response Function Acr Rows" , "7"  , "BCS-N"  }, //9
    {"SPT_RESP_UNIT_ROW"              , "Units of Row Spatial Response"      , "1"  , "BCS-A"  }, //10
    {"SPT_RESP_COL"                   , "Spatial Response Function Acr Cols" , "7"  , "BCS-N"  }, //11
    {"SPT_RESP_UNIT_COL"              , "Units of Col Spatial Response"      , "1"  , "BCS-A"  }, //12
    {"DATA_FLD_1"                     , "Reserved for Future Use"            , "48" , "UINT"   }, //13
    {"EXISTENCE_MASK"                 , "Bit_Wise Existence Mask Field"      , "4"  , "UINT"   }, //14
    {"RADIOMETRIC_ADJUSTMENT_SURFACE" , "Adjustment Surface"                 , "21" , "BCS-A"  }, //15
    {"ATMOSPHERIC_ADJUSTMENT_ALTITUDE", "Adjustment Altitude Above WGS84 Ell", "4"  , "BCS-A"  }, //16
    {"DIAMETER"                       , "Diameter of Lens"                   , "4"  , "BCS-N"  }, //17
    {"DATA_FLD_2"                     , "Reserved for Future Use"            , "20" , "UINT"   }, //18
    {"WAVE_LENGTH_UNIT"               , "Unit of measure for wavelength"     , "1"  , "BCS-A"  }, //19
    {"REMAINDER OF TRE"               , "Remainder of TRE"                   , "-1" , "DUMP"  }

};


const int BLOCKA_ICD_SIZE = 5;
std::string BLOCKA_ICD_Reference[BLOCKA_ICD_SIZE][4] = {
    {"BLOCK_INSTANCE"                 , "Block Instance"                     , "2"  , "BCS-N" }, //0
    {"N_GRAY"                         , "Number of Gray Fill Pixels"         , "5"  , "BCS-N" }, //1
    {"L_LINES"                        , "Row Count"                          , "5"  , "BCS-N" }, //2
    {"LAYOVER_ANGLE"                  , "Layover Angle"                      , "3"  , "BCS-A" }, //3
    {"REMAINDER OF TRE"               , "Remainder of TRE"                   , "-1" , "DUMP"  }
};


const int BNDPLB_ICD_SIZE = 2;
std::string BNDPLB_ICD_Reference[BNDPLB_ICD_SIZE][4] = {
    
    {"NUM_PTS"                        , "Number of points in bounding polygon" , "4" , "BCS-N"}, //0
    {"REMAINDER OF TRE"               , "REMAINDER OF TRE"                     , "-1" , "DUMP"}
};

const int ENGRDA_ICD_SIZE = 3;
std::string ENGRDA_ICD_Reference[ENGRDA_ICD_SIZE][4] = {
    {"RESRC"                          , "Unique Source System Name"            , "20" , "BCS-A"}, //0
    {"RECNT"                          , "Record Entry Count"                   , "3"  , "BCS-N"}, //1
    {"REMAINDER OF TRE"               , "REMAINDER OF TRE"                     , "-1" , "DUMP"}
};

#endif

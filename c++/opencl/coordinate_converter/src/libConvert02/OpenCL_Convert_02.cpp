//
// Created by Marvin Smith on 1/1/18.
//

#include "OpenCL_Convert_02.hpp"



/********************************/
/*          Constructor         */
/********************************/
OpenCL_Converter_Config_02::OpenCL_Converter_Config_02( const cl_device_type& device_type )
  : Coordinate_Converter_Config_Base(),
    m_class_name("OpenCL_Converter_Config_02"),
    m_ocl_device_type(device_type)
{
}

/********************************/
/*          Constructor         */
/********************************/
OpenCL_Converter_02::OpenCL_Converter_02(OpenCL_Converter_Config_02::ptr_t config)
 : Coordinate_Converter_Base(config),
   m_class_name("OpenCL_Converter_02")
{
    // Cast the Configuration
    m_config = std::dynamic_pointer_cast<OpenCL_Converter_Config_02>(config);

    // Create the OpenCL Context Objects
    Initialize_OpenCL();
}


/********************************/
/*          Destructor          */
/********************************/
OpenCL_Converter_02::~OpenCL_Converter_02()
{
    Finalize_OpenCL();
}


/************************************************/
/*          Convert a single coordinate         */
/************************************************/
Coordinate_UTM  OpenCL_Converter_02::Convert_To_UTM( const Coordinate_Geographic& coordinate )
{
    // Create output list
    Coordinate_UTM output_coord ;

    //converts lat/long to UTM coords.  Equations from USGS Bulletin 1532
    //East Longitudes are positive, West longitudes are negative.
    //North latitudes are positive, South latitudes are negative
    //Lat and Long are in decimal degrees
    //Written by Chuck Gantz- chuck.gantz@globalstar.com

    bool found;
    auto ellipsoid = Get_Ellipsoid( DatumType::WGS84, found );
    if( !found ){
        return output_coord;
    }

    bool IsNorth = true;
    if( coordinate.Get_Latitude_Degrees() < 0 ){
        IsNorth = false;
    }

    double a = ellipsoid.Get_Equatorial_Radius();
    double eccSquared = ellipsoid.Get_Eccentricity_Squared();
    double k0 = 0.9996;

    double LongOrigin;
    double eccPrimeSquared;
    double N, T, C, A, M;

    //Make sure the longitude is between -180.00 .. 179.9
    double LongTemp = (coordinate.Get_Longitude_Degrees()+180)-int((coordinate.Get_Longitude_Degrees()+180)/360)*360-180; // -180.00 .. 179.9;

    double LatRad = coordinate.Get_Latitude_Degrees()*M_PI/180.0;
    double LongRad = LongTemp*M_PI/180.0;
    double LongOriginRad;
    int    ZoneNumber;

    ZoneNumber = int((LongTemp + 180)/6) + 1;

    if( coordinate.Get_Latitude_Degrees() >= 56.0 && coordinate.Get_Latitude_Degrees() < 64.0 && LongTemp >= 3.0 && LongTemp < 12.0 )
        ZoneNumber = 32;

    // Special zones for Svalbard
    if( coordinate.Get_Latitude_Degrees() >= 72.0 && coordinate.Get_Latitude_Degrees() < 84.0 )
    {
        if(      LongTemp >= 0.0  && LongTemp <  9.0 ) ZoneNumber = 31;
        else if( LongTemp >= 9.0  && LongTemp < 21.0 ) ZoneNumber = 33;
        else if( LongTemp >= 21.0 && LongTemp < 33.0 ) ZoneNumber = 35;
        else if( LongTemp >= 33.0 && LongTemp < 42.0 ) ZoneNumber = 37;
    }
    LongOrigin = (ZoneNumber - 1)*6 - 180 + 3;  //+3 puts origin in middle of zone
    LongOriginRad = LongOrigin * M_PI/180.0;

    //compute the UTM Zone from the latitude and longitude
    //sprintf(UTMZone, "%d%c", ZoneNumber, UTMLetterDesignator(Lat));

    eccPrimeSquared = (eccSquared)/(1-eccSquared);

    N = a/sqrt(1-eccSquared*sin(LatRad)*sin(LatRad));
    T = tan(LatRad)*tan(LatRad);
    C = eccPrimeSquared*cos(LatRad)*cos(LatRad);
    A = cos(LatRad)*(LongRad-LongOriginRad);

    M = a*((1   - eccSquared/4      - 3*eccSquared*eccSquared/64    - 5*eccSquared*eccSquared*eccSquared/256)*LatRad
           - (3*eccSquared/8   + 3*eccSquared*eccSquared/32    + 45*eccSquared*eccSquared*eccSquared/1024)*sin(2*LatRad)
           + (15*eccSquared*eccSquared/256 + 45*eccSquared*eccSquared*eccSquared/1024)*sin(4*LatRad)
           - (35*eccSquared*eccSquared*eccSquared/3072)*sin(6*LatRad));

    double UTMEasting = (double)(k0*N*(A+(1-T+C)*A*A*A/6
                                       + (5-18*T+T*T+72*C-58*eccPrimeSquared)*A*A*A*A*A/120)
                                 + 500000.0);

    double UTMNorthing = (double)(k0*(M+N*tan(LatRad)*(A*A/2+(5-T+9*C+4*C*C)*A*A*A*A/24
                                                       + (61-58*T+T*T+600*C-330*eccPrimeSquared)*A*A*A*A*A*A/720)));

    if(coordinate.Get_Latitude_Degrees() < 0)
        UTMNorthing += 10000000.0; //10000000 meter offset for southern hemisphere

    // return the output coordinate
    return Coordinate_UTM( ZoneNumber, IsNorth, UTMEasting, UTMNorthing);
}


/****************************************************************/
/*          Convert a Batch Collection of Coordinates           */
/****************************************************************/
std::vector<Coordinate_UTM>  OpenCL_Converter_02::Convert_To_UTM( const std::vector<Coordinate_Geographic>& coordinates)
{
    // Create output list
    std::vector<Coordinate_UTM> output_list;

    for( auto incoord : coordinates ){
        output_list.push_back(Convert_To_UTM(incoord));
    }
    return output_list;
}


/****************************************************/
/*          Initialize the OpenCL Contexts          */
/****************************************************/
void OpenCL_Converter_02::Initialize_OpenCL()
{
    // Misc Vars
    cl_int status = 0;

    //Get the first available platform. Use it as the default platform
    status = clGetPlatformIDs(1, &m_ocl_platform, NULL);
    LOG_OCL_ERROR(status, "clGetPlatformIDs Failed." );

    //Get the first available device
    status = clGetDeviceIDs (m_ocl_platform, m_config->Get_OCL_Device_Type(), 1, &m_ocl_device, NULL);
    LOG_OCL_ERROR(status, "clGetDeviceIDs Failed." );
}

/****************************************************/
/*          Finalize the OpenCL Contexts            */
/****************************************************/
void OpenCL_Converter_02::Finalize_OpenCL()
{

}

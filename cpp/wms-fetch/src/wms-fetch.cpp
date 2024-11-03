/**
 * @file    wms-fetch.cpp
 * @author  Marvin Smith
 * @date    1/27/2015
*/

// C++ Libraries
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <curl/curl.h>


using namespace std;

std::string foo;

size_t Write_Callback( char* buffer, size_t size, size_t nmemb, void* up )
{

    foo += std::string(buffer).substr(0,size*nmemb);
    return (size*nmemb);
}


int main( int argc, char* argv[] )
{
    
    // Create URL
    //std::string url = "http://webmap.ornl.gov/cgi-bin/mapserv?&BBOX=-180,-90,180,90&SERVICE=WMS&HEIGHT=250&REQUEST=GetMap&STYLES=default&WIDTH=500&EXCEPTIONS=application/vnd.ogc.se_xml&TRANSPARENT=true&VERSION=1.1.1&FORMAT=image/png&map=/sdat/config/mapfile//980/980_13_wms.map&LAYERS=980_13_band60&originator=SDAT&SRS=EPSG:4326";
    std::string url = "http://webmap.ornl.gov/ogcbroker/wms?SERVICE=WMS&VERSION=1.1.1&REQUEST=GetCapabilities";
    
    // Initialize Curl
    CURL *image;
    CURLcode imgresult;
    FILE* fp;

    curl_global_init(CURL_GLOBAL_ALL);
    image = curl_easy_init();

    // Type info
    char* typeinfo;

    
    // Make sure the object was loaded
    if( image )
    {
        
        //   Init
        curl_easy_setopt( image, CURLOPT_URL, url.c_str());
        imgresult = curl_easy_perform(image);
        
        // Get the Type
        curl_easy_getinfo( image, CURLINFO_CONTENT_TYPE, &typeinfo);
        std::cout << "Type: " << typeinfo << std::endl;
        
        std::cout << "Press Enter To Continue" << std::endl;
        cin.get();
        
        curl_easy_setopt( image, CURLOPT_WRITEFUNCTION, &Write_Callback);
        imgresult = curl_easy_perform(image);

        // Get image
        std::cout << "Grabbing the image" << std::endl;
        imgresult = curl_easy_perform(image);
        if( imgresult ) {
            cout << "Couldn't grab the image" << std::endl;
        }
   
    } 
    else{
        std::cout << "Failed to initialize CURL" << std::endl;
    }

    std::cout << "Wrote: " << foo << std::endl;

    // Clean up
    std::cout << "Cleaning Up" << std::endl;
    curl_easy_cleanup(image);
    curl_global_cleanup();
    

    return 0;
}


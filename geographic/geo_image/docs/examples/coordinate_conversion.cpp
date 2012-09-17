#include <GeoImage.h>


using namespace std;

int main( int argc, char* argv[] ){

    //Load a GeoImage
    GEO::GeoImage img( argv[1], true );

    //find the four corners
    GEO::CoordinateLatLon  tl, br;
    img.get_corner_coordinates( tl, br);

    cout << "TL: " << tl.toString() << endl;
    cout << "BR: " << br.toString() << endl;
    
    //convert the corners to UTM
    //GEO::CoordinateUTM   utmTL, utmBR;

    //create a few different standalone coordinates


    //modify the coordinates


    //find the distance between two points


    //interpolate between the points


    return 0;
}

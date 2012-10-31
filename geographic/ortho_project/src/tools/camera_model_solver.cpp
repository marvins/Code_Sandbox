#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "camera_model_solver/Fitness_Functor.hpp"
#include "camera_model_solver/Genetic_Algorithm.hpp"

#include <GeoImage.hpp>

using namespace cv;
using namespace std;


const int     POPULATION_SIZE    = 100000;
const int     PRESERVATION_COUNT = 100;
const double  SELECTION_RATE     = 0.8;

vector<Point> image_values;
vector<Point3f> earth_values;

string point_filename;
string image_filename;

Size global_image_size;


//double pixel2world( Point const& pix, Point3f const& expected, Variables const& vars );

void parse_pointfile( );

int main( int argc, char* argv[] ){
    
    try{
        
        double threshold = -0.01;

        // initialize the random number generator
        srand( time( NULL ) );
    
        // parse the command line options
        point_filename = argv[1];
        //image_filename = argv[2];
        
        // load the test image
        //Mat image = imread( image_filename.c_str(), 0 );

        // load the point file
        parse_pointfile();
    
        Variables vars;
        vars.focal_length=1;
        vars.image_plane = Point2f( 1, 1);
        vars.rotation_angle = 0;
        vars.rotation_axis.resize(3);
        vars.rotation_axis[0] = 1;
        vars.rotation_axis[1] = 0;
        vars.rotation_axis[2] = 0;
        vars.camera_position = Point3f(0,0,1000);
        
        // create a fitness functor to store our evaluation methods
        Fitness_Functor fitness_functor( image_values, earth_values, Size(1000,1000) );

        // initialize the Genetic Algorithm
        GA::GA genetic_algorithm( fitness_functor, MAX_GENOME_LENGTH, POPULATION_SIZE, PRESERVATION_COUNT, SELECTION_RATE );
    
        // iterate until you have a solution
        for( int i=0; i<1000000; i++ ){

            // selection
            genetic_algorithm.selection();
            genetic_algorithm.print();
            
            if( genetic_algorithm.best_fitness() > threshold )
                break;

            // crossover
            genetic_algorithm.crossover();

            // evolution
            genetic_algorithm.mutation();
        
            // check the fitness function
            /**
             * first query each data string
             * - convert the data string into a variable object
             * - evaluate the variable container against each input/output pair
             * - 
            */

        }
        
        cout << "Optimized Result" << endl;
        genetic_algorithm.print();

    } catch( string e ){
        cout << e << endl;
    }

    return 0;
}


void parse_pointfile( ){

    ifstream fin;
    fin.open(point_filename.c_str() );


    string line;
    getline( fin, line);

    while( !fin.eof() ){

        // check for tag
        if( line.substr(0,6) == "POINT:"){
            
            // remove tag
            line = line.substr(6);
            
            // parse values
            vector<string> items = GEO::STR::string_split(GEO::STR::string_trim(line), ", ");
            
            Point tmpPnt;
            Point3f tmpPnt3;

            tmpPnt.x = GEO::STR::str2num<int>(GEO::STR::string_trim(items[0]));
            tmpPnt.y = GEO::STR::str2num<int>(GEO::STR::string_trim(items[1]));
            tmpPnt3.y = GEO::STR::str2num<double>(GEO::STR::string_trim(items[2]));
            tmpPnt3.x = GEO::STR::str2num<double>(GEO::STR::string_trim(items[3]));
            tmpPnt3.z = GEO::STR::str2num<double>(GEO::STR::string_trim(items[4]));
            
            image_values.push_back( tmpPnt );
            earth_values.push_back( tmpPnt3 );

        }
        getline( fin, line );

    }


    fin.close();

}


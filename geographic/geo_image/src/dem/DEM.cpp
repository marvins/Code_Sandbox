#include "DEM.hpp"

#include <iostream>
#include <string>


using namespace cv;
using namespace std;

namespace GEO{

    DEM_Params::DEM_Params( const int& ftype, std::string const& root_dir ){

        filetype      = ftype;
        dted_root_dir = root_dir;
    }// end of DEM_Params constructor
    

    
    /**
     * Default Constructor
    */
    DEM::DEM( ){
    
    }


    /**
     * Parameterized Constructor given a pre-existing DEM Tile and both corners
    */
    DEM::DEM( const Point2f& _tl, const Point2f& _br, const Mat& data ){
        
        //set the image corners
        m_tl = _tl;
        m_br = _br;
        
        //set the tile data
        tile = data.clone();

    }

    /*
    //DEM Constructor
    DEM::DEM( double const& tl_lat, double const& tl_lon, 
              double const& br_lat, double const& br_lon, 
              DEM_Params const& params ){
        
        /** need to start looking at how many files we need
         * 
         * 1.  DTED uses 1 deg x 1 deg grids
         *
        double tempD;
        if( params.filetype == DTED ){


            //compute range
            br.x = max( tl_lon, br_lon);  br.y = min( tl_lat, br_lat);  
            tl.x = min( tl_lon, br_lon);  tl.y = max( tl_lat, br_lat);
            
            if( fabs(tl.x)-std::floor(fabs(tl.x)) < 0.00001 )
                tl.x += 0.00001;

            //check to see if we have one file or multiple files
            //build list of images required
            int lat_needed = 1 + fabs( std::floor(tl.y) - std::floor(br.y) );
            int lon_needed = 1 + fabs( std::floor(tl.x) - std::floor(br.x) );
            
            //create 2d array of final images
            vector<vector<Mat> > image_set(lon_needed);
            for( int i=0; i<lon_needed; i++)
                image_set[i] = vector<Mat>(lat_needed);

            
            //begin loading sub tiles
            for( int j=0; j<lat_needed; j++ ){
                for( int i=0; i<lon_needed; i++ ){
                    
                    //compute the required filename
                    string exp_filename = DTEDUtils::coordinate2filename( 
                            /** Lat *std::floor(br.y)+(lat_needed - j - 1)+0.0001, 
                            /** Lon *std::floor(tl.x)+(i)+0.0001 
                            );
                    
                    string act_filename = params.dted_root_dir + "/" + exp_filename;
                    
                    //make sure the filename exists
                    if( bf::exists(bf::path(act_filename)) == false ){
                        throw string("Error: File does not exist");
                    }
                    
                    //load crop
                    Mat subcrop = GEO::GeoImage( act_filename, true ).get_image();
                    
                    //make sure that pixel type is proper
                    if( subcrop.type() != CV_16SC1 )
                        throw string("Image must be CV_16SC1" );
                   
                    
                    //get crop range
                    pair<double,double> lat_ran( std::max( std::floor(tl.y-j), br.y ), std::min( std::ceil(tl.y-j), tl.y));
                    pair<double,double> lon_ran( std::max( std::floor(tl.x+i), tl.x ), std::min( std::ceil(tl.x+i), br.x));
                    

                    //compute the percentage of the image you are using
                    pair<double,double> lat_pct( lat_ran.first - std::floor(lat_ran.first), 1 - (std::ceil(lat_ran.second) - lat_ran.second));
                    pair<double,double> lon_pct( lon_ran.first - std::floor(lon_ran.first), 1 - (std::ceil(lon_ran.second) - lon_ran.second));
                  
                    tempD  = 1 - lat_pct.first;
                    lat_pct.first = 1 - lat_pct.second;
                    lat_pct.second= tempD;

                   //given the percentage, compute the pixel range you need
                    pair<int,int> lat_img_ran( lat_pct.first*subcrop.rows, lat_pct.second*subcrop.rows);
                    pair<int,int> lon_img_ran( lon_pct.first*subcrop.cols, lon_pct.second*subcrop.cols);
                    
                    //create the actual final image
                    Mat crop( lat_img_ran.second-lat_img_ran.first, 
                              lon_img_ran.second-lon_img_ran.first, 
                              subcrop.type());

                    //load pixels
                    for( int x=lon_img_ran.first; x<lon_img_ran.second; x++ ){
                    for( int y=lat_img_ran.first; y<lat_img_ran.second; y++ )
                            
                            if( subcrop.type() == CV_8UC1 ){
                                crop.at<uchar>(y-lat_img_ran.first, x-lon_img_ran.first) = subcrop.at<uchar>(y,x);
                            }
                            else if( subcrop.type() == CV_16UC1 ){
                                crop.at<ushort>(y-lat_img_ran.first, x-lon_img_ran.first) = subcrop.at<ushort>(y,x);
                            }
                            else if( subcrop.type() == CV_16SC1 ){
                                crop.at<short>(y-lat_img_ran.first, x-lon_img_ran.first) = subcrop.at<short>(y,x);
                            }
                            else
                                throw string("TYPE NOT SUPPORTED");
                        }
                    
                    //add final image to image set
                    image_set[i][j] = crop.clone();
                
                }//end i
            }//end j

            /** We have now compiled a list of required tiles 
             *  - Now we need to merge them into a single image
             *

            //compute the expected width and height of the image
            int final_x = 0;
            int final_y = 0;
            for( int i=0; i<lon_needed; i++ )
                final_x += image_set[i][0].cols;
            for( int i=0; i<lat_needed; i++ )
                final_y += image_set[0][i].rows;
        
            //create final tile
            tile = Mat( Size( final_x, final_y), image_set[0][0].type() );
            tile = Scalar(0);

            int cx = 0;
            int cy = 0;

            //start loading each image in image set into final tile
            for( int i=0; i<(int)image_set.size(); i++){
                cy = 0;
                for( int j=0; j<(int)image_set[i].size(); j++){
                    
                    //iterate over specific image
                    for( int x =0; x < image_set[i][j].cols; x++ ){
                        for( int y =0; y < image_set[i][j].rows; y++ ){
                            
                            if( y+cy >= tile.rows ){ 
                                cout << x << ", " << y << " vs " << cx << ", " << cy << endl;
                                throw string("ROWS FAILED"); }
                            if( x+cx >= tile.cols ){ 
                                cout << x << ", " << y << " vs " << cx << ", " << cy << endl;
                                throw string("COLS FAILED"); }

                            if( image_set[0][0].type() == CV_16UC1 )
                                tile.at<ushort>( y+cy, x+cx) = image_set[i][j].at<ushort>( y, x);
                            else if( image_set[0][0].type() == CV_16SC1 )
                                tile.at<short>(  y+cy, x+cx) = image_set[i][j].at<short>(  y, x);
                            else
                                throw string("ERROR: unsupported dem format");

                        }}

                    //update cx, cy
                    cy += image_set[0][j].rows;
                }
                cx += image_set[i][0].cols;
            }

        }
        else{
            throw std::string("Error: unsupported DEM format");
        }

    }

    DEM::DEM( cv::Point2f const& point, DEM_Params const& params ){

        //compute the proper dted filename
        string exp_filename = DTEDUtils::coordinate2filename( 
                /** Lat *std::floor(point.y)+0.0001, 
                /** Lon *std::floor(point.x)+0.0001 );
        
        //append the dted root directory
        string act_filename = params.dted_root_dir + "/" + exp_filename;

        //load the GeoImage
        GEO::GeoImage gimg( act_filename, true );

        //since we are dealing with DTED, we want 1 deg by 1 deg squares
        int pctX = (     point.x - std::floor(point.x)) *3601;
        int pctY = (1 - (point.y - std::floor(point.y)))*3601;
        
        current   = Point2f( pctX, pctY);
        elevation = (gimg.get_image().at<short>(pctY, pctX));
        
    }

    /**
     * Destructor
     */
    DEM::~DEM(){
    
    }

    /**
     * Pull the elevation data as an OpenCV Mat
     */
    cv::Mat DEM::get_tile()const{

        if( !tile.data )
            return Mat();
        return tile.clone();
    } //end of get_raw function


    /**
     * Return the value and location of the highest elevation
     * in the tile.
     *
    double DEM::max_elevation( double& lat, double& lon )const{

        double _max = 0;
        int I = 0, J = 0;
        for( size_t i=0; i<tile.cols; i++)
            for( size_t j=0; j<tile.rows; j++ )
                if( tile.type() == CV_16SC1 ){
                    if( tile.at<short>(j,i) > _max ){
                        _max = tile.at<short>(j,i);
                        I = i;
                        J = j;
                    }
                }
        lat = J;
        lon = I;

        return _max;
    }

    double DEM::get_elevation()const{
        return elevation;
    }


Vec3b color_relief( double elevation, double minC, double maxC ){

    double maxR = maxC;
    double minR = minC;

    double x = ((maxR - minR)-(elevation - minR))/(maxR - minR)*255;
    double y = 255 - x;
    double z = 0;

    if( x > 255 ) x = 255;
    if( y > 255 ) y = 255;
    if( z > 255 ) z = 255;
    if( x < 0 )   x = 0;
    if( y < 0 )   y = 0;
    if( z < 0 )   z = 0;
    
    return Vec3b( x, y, z);
}



Mat DEM::relief_map()const{

    //create a 3 channel image
    Mat output( tile.size(), CV_8UC3);

    //iterate over the image applying the new pixel values
    for( size_t i=0; i<tile.cols; i++)
    for( size_t j=0; j<tile.rows; j++)
        output.at<Vec3b>(j,i) = color_relief( tile.at<short>(j,i), 1000, 4450 );

    return output;
    
}

Point DEM::get_pixel_coordinate( Point2f const& coordinate ){

    //find the percentage of the image from the tl to br
    double pctX = tile.cols*(coordinate.x - tl.x )/(br.x - tl.x);
    double pctY = tile.rows*(1.0-(coordinate.y - br.y )/(tl.y - br.y));

    return Point( pctX, pctY);

}

*/
    
    Point2f DEM::ne()const{  return Point2f( m_br.x, m_tl.y ); }
    Point2f DEM::nw()const{  return Point2f( m_tl.x, m_tl.y ); }
    Point2f DEM::se()const{  return Point2f( m_br.x, m_br.y ); }
    Point2f DEM::sw()const{  return Point2f( m_tl.x, m_br.y ); }

}//end of GEO namespace


#ifndef __SRC_FITNESS_FUNCTOR_HPP__
#define __SRC_FITNESS_FUNCTOR_HPP__

#include <opencv2/core/core.hpp>

#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>


const int MAX_GENOME_LENGTH=104;


/**
 * String output utilities
*/
void printChars( std::ostream& ostr, int numCh, char chOut );
void printString( std::ostream& ostr, const std::string &stringVal, int blockSize, const std::string &justify );


/**
 * Convert a boolean array into an integer
*/
int bits2int( std::vector<bool>const& genome, const int& start, const int& length );

/**
 * Convert a boolean array into a double
*/
double convert_bits2double( std::vector<bool>const& genome, const int& start, const int& length, 
                            const double& step, const double& _min, const double& _max );


/**
 * This is the class which holds the required parameters for our class
*/
class Variables{

    public:
        
        Variables( );

        /** 
         * Constructor which converts the genome into parameters.
        */
        Variables( std::vector<bool> const& genome, const cv::Point3f& camera_origin );
        
        /**
         * 
        */
        void print( std::ostream& ostr )const;

        
        int total_length;

        double              focal_length;
        cv::Point2f         image_plane;
        double              rotation_angle;
        std::vector<double> rotation_axis;
        cv::Point3f         camera_position;

};

class Fitness_Func_Sort{
    
    public:
        bool operator()( std::pair<std::vector<bool>,double>const& a, std::pair<std::vector<bool>,double>const& b )const;

};


class Fitness_Functor{

    public:
        
        Fitness_Functor( const std::vector<cv::Point>&   img_pnts, 
                         const std::vector<cv::Point3f>& earth_pnts,
                         const cv::Size& img_size,
                         const cv::Point3f& cam_origin );
        
        /**
         * Fitness Function
         *
         * @param[in] genome Genome strain to test against.
        */
        double operator()( std::vector<bool>const& genome )const;
        
        
        double operator()( Variables const& var )const;
        
        void print_vars( std::ostream& ostr, std::vector<bool>const& str )const;
        
        /** 
         * Print the hard-coded values initialized
        */
        void print_config( std::ostream& ostr )const;

    private:
        
        /** This is the set of input image points */
        const std::vector<cv::Point> image_points;

        /** This is the set of output earth points */
        const std::vector<cv::Point3f> earth_points;
        
        /** This is the image size */
        const cv::Size image_size;

        /** This is the camera origin */
        const cv::Point3f camera_origin;
};


#endif

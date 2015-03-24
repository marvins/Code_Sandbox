#ifndef __LAS_DEMO_POINT_EXTRACTOR_HPP__
#define __LAS_DEMO_POINT_EXTRACTOR_HPP__

// OpenCV Libraries
#include <opencv2/core/core.hpp>

// Boost Libraries
#include <boost/filesystem.hpp>

// LibLas Libraries
#include <liblas/liblas.hpp>

// C++ Standard Libraries
#include <deque>
#include <fstream>
#include <iostream>

/**
 * @class PointExtractor
*/
class PointExtractor{

    public:
        
        /**
         * @brief Constructor
         *
         * @param[in] pathname Path of file to open.
        */
        PointExtractor( boost::filesystem::path const& pathname );

        /**
         * @brief Get the point list
         */
        inline std::deque<cv::Point3d> Get_Point_List()const{
            return m_point_list;
        }

        /**
         * @brief Get the min point
        */
        inline cv::Point2d Get_Min_Point()const{
            return m_min_point;
        }


        /**
         * @brief Get the max point
        */
        inline cv::Point2d Get_Max_Point()const{
            return m_max_point;
        }


    private:

        /// Point List
        std::deque<cv::Point3d> m_point_list;

        /// LibLas Header
        liblas::Header m_lidar_header;

        /// LibLas Spatial Reference
        liblas::SpatialReference m_lidar_srs;

        /// Min Point
        cv::Point2d m_min_point;

        /// Max Point
        cv::Point2d m_max_point;

}; // End of PointExtractor Class


#endif

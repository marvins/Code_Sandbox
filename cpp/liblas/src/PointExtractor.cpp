#include "PointExtractor.hpp"

// C++ Libraries
#include <stdexcept>

using namespace std;

/**
 * Constructor
*/
PointExtractor::PointExtractor( boost::filesystem::path const& pathname )
{

    // Make sure the file exists
    if( boost::filesystem::exists(pathname) == false ){
        throw std::runtime_error("Path does not exist.");
    }

    // Open the file
    std::ifstream ifs;
    ifs.open(pathname.c_str(), std::ios::binary | std::ios::in );

    // Create the las factory
    liblas::ReaderFactory f;
    liblas::Reader reader = f.CreateWithStream(ifs);

    // Grab header information
    m_lidar_header = reader.GetHeader();

    m_lidar_srs = m_lidar_header.GetSRS();
    std::cout << m_lidar_srs.GetProj4() << std::endl;
    
    // Get the extent
    m_min_point.x = m_lidar_header.GetMinX();
    m_min_point.y = m_lidar_header.GetMinY();
    m_max_point.x = m_lidar_header.GetMaxX();
    m_max_point.y = m_lidar_header.GetMaxY();

    // Read points
    while (reader.ReadNextPoint())
    {
        liblas::Point const& p = reader.GetPoint();
        m_point_list.push_back(cv::Point3d(p.GetX(), p.GetY(), p.GetZ()));
    }

    std::cout << "Points count: " << m_lidar_header.GetPointRecordsCount() << '\n';
    std::cout << "Range: " << m_min_point << ", " << m_max_point << std::endl;

    // close the file
    ifs.close();
}



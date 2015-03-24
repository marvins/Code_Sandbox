#include "GridMarker.hpp"

/**
 * Constructor
*/
GridMarker::GridMarker( )
  : m_min_z(-1),
    m_max_z(-1),
    m_z_values()
{
}

/**
 * Add Z
*/
void GridMarker::Add( const double& z_value )
{
    // initialize if unset
    if( m_z_values.size() == 0 ){
        m_min_z = m_max_z = z_value;
    }

    // update min/max
    m_min_z = std::min(z_value, m_min_z);
    m_max_z = std::max(z_value, m_max_z);

    // Update the z value
    m_z_values.push_back(z_value);
}

void GridMarker::Set_Z(){

    // If the value is zero, skip
    if( m_z_values.size() <= 0 ){ return; }

    // Otherwise, compute the mean
    double sum = 0;
    for( int z=0; z<m_z_values.size(); z++ ){
        sum += m_z_values[z];
    }

    m_mean_z = sum / (m_z_values.size());

}

/**
 * To String
*/
std::string GridMarker::ToString()const{

    // Create stream
    std::stringstream sin;

    // Create output
    sin << "GridMarker:" << std::endl;
    sin << "   Min Z: " << m_min_z << std::endl;
    sin << "   Max Z: " << m_max_z << std::endl;
    sin << "   Count: " << m_z_values.size() << std::endl;
   
    return sin.str();
}


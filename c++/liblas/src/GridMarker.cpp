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


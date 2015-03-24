#ifndef __GRID_MARKER_HPP__
#define __GRID_MARKER_HPP__

// C++ Standard Libraries
#include <deque>

/**
 * @class GridMarker
*/
class GridMarker{

    public:
        
        /**
         * @Constructor
        */
        GridMarker();

        /**
         * @brief Add to marker
        */
        void Add( const double& z_value );

    private:
        
        /// Min Z
        double m_min_z;

        /// Max Z
        double m_max_z;

        /// List of z's
        std::deque<double> m_z_values;


}; // End of GridMarker Class


#endif

#ifndef __GRID_MARKER_HPP__
#define __GRID_MARKER_HPP__

// C++ Standard Libraries
#include <deque>
#include <sstream>
#include <string>

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
        
        /**
         * @brief print to string
        */
        std::string ToString()const;
        
        /**
         * Get the number of markers
        */
        inline int Get_Z_Count()const{
            return m_z_values.size();
        }

        /**
         * Set Z Post Adding
        */
        void Set_Z();

        inline double Get_Z(bool& set)const{
            if( m_z_values.size() > 0 ){
                set = true;
            }else{
                set = false;
            }
           
            return m_mean_z;
        }


    private:
        
        /// Min Z
        double m_min_z;

        /// Max Z
        double m_max_z;

        /// List of z's
        std::deque<double> m_z_values;

        /// Computed Z Value
        double  m_mean_z;

}; // End of GridMarker Class


#endif

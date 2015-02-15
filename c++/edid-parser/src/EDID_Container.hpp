#ifndef __EDID_CONTAINER_HPP__
#define __EDID_CONTAINER_HPP__


/**
 * @class EDID_Entry
*/
class EDID_Entry{


}; /// End of EDID_Entry Class


/**
 * @class EDID_Container
*/
class EDID_Container{

    public:
        
        /**
         * Constructor
        */
        EDID_Container( const std::vector<uint8_t>& edid_data );

    
    private:
        
        /// Raw Data
        std::vector<uint8_t> m_raw_data;

        /// Edid Entries
        std::vector<EDID_Field> m_edid_fields;

}; /// End of EDID_Container Class


#endif


#ifndef __J2K_PARSERS_HPP__
#define __J2K_PARSERS_HPP__

// C++ Libraries
#include <cinttypes>
#include <deque>
#include <memory>
#include <vector>


/**
 * @class J2K_Marker_Base
 */
class J2K_Marker_Base
{
    public:
        
        /// Pointer Type
        typedef std::shared_ptr<J2K_Marker_Base> ptr_t;
        
        /**
         * @brief Constructor
         */
        J2K_Marker_Base();


        /**
         * @brief Matches
         */
        virtual bool Contains( const uint8_t* buffer )const;


        /**
         * @brief Process
         */
        virtual void Process( const uint8_t* buffer,
                              int64_t&       index ) = 0;

    protected:
        
        /// Start of Tag Marker
        std::string m_marker_tag;

}; // End of J2K_Marker_Base Class


/**
 * @class J2K_SOC_Marker
 */
class J2K_SOC_Marker : public J2K_Marker_Base
{
    public:
 
        /**
         * @brief Constructor
         */
        J2K_SOC_Marker();

        
        /**
         * @brief Process
         */
        virtual void Process( const uint8_t* buffer,
                              int64_t&       index );


}; // End of J2K_SOC_Marker Class


/**
 * @class J2K_SOD_Marker
 */
class J2K_SOD_Marker : public J2K_Marker_Base
{
    public:
 
        /**
         * @brief Constructor
         */
        J2K_SOD_Marker();

        
        /**
         * @brief Process
         */
        virtual void Process( const uint8_t* buffer,
                              int64_t&       index );


}; // End of J2K_SOC_Marker Class


/**
 * @class J2K_SOT_Marker
 */
class J2K_SOT_Marker : public J2K_Marker_Base
{
    public:
 
        /**
         * @brief Constructor
         */
        J2K_SOT_Marker();


        /**
         * @brief Process
         */
        virtual void Process( const uint8_t* buffer,
                              int64_t&       index );


}; // End of J2K_SOT_Marker Class


/**
 * @class J2K_SIZ_Marker
 */
class J2K_SIZ_Marker : public J2K_Marker_Base
{
    public:
 
        /**
         * @brief Constructor
         */
        J2K_SIZ_Marker();

        
        /**
         * @brief Process
         */
        virtual void Process( const uint8_t* buffer,
                              int64_t&       index );


}; // End of J2K_SIZ_Marker Class



/**
 * @class J2K_COC_Marker
 */
class J2K_COC_Marker : public J2K_Marker_Base
{
    public:
 
        /**
         * @brief Constructor
         */
        J2K_COC_Marker();

        
        /**
         * @brief Process
         */
        virtual void Process( const uint8_t* buffer,
                              int64_t&       index );


}; // End of J2K_COC_Marker Class


/**
 * @class J2K_COD_Marker
 */
class J2K_COD_Marker : public J2K_Marker_Base
{
    public:
 
        /**
         * @brief Constructor
         */
        J2K_COD_Marker();

        
        /**
         * @brief Process
         */
        virtual void Process( const uint8_t* buffer,
                              int64_t&       index );


}; // End of J2D_COM_Marker Class



/**
 * @class J2K_COM_Marker
 */
class J2K_COM_Marker : public J2K_Marker_Base
{
    public:
 
        /**
         * @brief Constructor
         */
        J2K_COM_Marker();

        
        /**
         * @brief Process
         */
        virtual void Process( const uint8_t* buffer,
                              int64_t&       index );


}; // End of J2K_COM_Marker Class


/**
 * @class J2K_EOC_Marker
 */
class J2K_EOC_Marker : public J2K_Marker_Base
{
    public:
 
        /**
         * @brief Constructor
         */
        J2K_EOC_Marker();


        /**
         * @brief Process
         */
        virtual void Process( const uint8_t* buffer,
                              int64_t&       index );

}; // End of J2K_EOC_Marker Class


/**
 * @brief J2K_Marker_Container
 */
class J2K_Marker_Container
{
    public:

        /**
         * @brief Constructor
         */
        J2K_Marker_Container();


        /**
         * @brief Process Markers
         */
        void Process_Markers( const uint8_t* buffer,
                              int64_t&       index,
                              const int64_t& max_size );


}; // End of J2K_Marker_Container
        


#endif

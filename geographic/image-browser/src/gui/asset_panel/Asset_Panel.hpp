/**
 * @file   Asset_Panel.hpp
 * @author Marvin Smith
 * @date   3/2/2018
 */
#ifndef GEO_DATA_MANAGER_GUI_ASSET_PANEL_HPP
#define GEO_DATA_MANAGER_GUI_ASSET_PANEL_HPP

// Qt Libraries
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>

// Project Libraries
#include "../../core/Geo_Options.hpp"


/**
 * @class Asset_Panel
 */
class Asset_Panel : public QWidget
{
    public:

        /**
         * @brief Constructor
         *
         * @param[in] options
         * @param[in] parent
         */
        Asset_Panel( const Geo_Options& options,
                     QWidget*           parent = nullptr );


    private:

        void Initialize_GUI();


        /**
         * @brief Build the Widget Toolbar
         */
        void Build_Toolbar();


        /// Class Name
        std::string m_class_name;

        /// Config Options
        Geo_Options m_options;

        /// Main Layout
        QVBoxLayout* m_main_layout;

        /// Asset View


        /// Toolbar


}; // End of Asset_Panel Class

#endif
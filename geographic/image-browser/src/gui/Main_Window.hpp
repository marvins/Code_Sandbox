/**
 * @file    Main_Window.hpp
 * @author  Marvin Smith
 * @date    3/2/2018
 */
#ifndef GEO_DATA_MANAGER_GUI_MAIN_WINDOW_HPP
#define GEO_DATA_MANAGER_GUI_MAIN_WINDOW_HPP

// Qt Libraries
#include <QMainWindow>
#include <QSplitter>

// Project Libraries
#include "../core/Geo_Options.hpp"
#include "asset_panel/Asset_Panel.hpp"

// ArcGIS Libraries
#include <Map.h>
#include <MapGraphicsView.h>


/**
 * @class Main_Window
 */
class Main_Window : public QMainWindow
{
    public:

        /**
         * @brief Constructor
         */
        Main_Window( const Geo_Options& options );


    private:

        /**
         * @brief Initialize the GUI
         */
        void Initialize_GUI();

        void Initialize_Map();


        /// Class Name
        std::string m_class_name;

        /// System Options
        Geo_Options m_options;

        /// Main Widget
        QSplitter* m_main_widget;

        /// Asset Panel Widget
        Asset_Panel* m_asset_panel;

        ///ArcGIS Map
        Esri::ArcGISRuntime::Map* m_map;
        Esri::ArcGISRuntime::MapGraphicsView* m_mapView;

}; // End of the Main_Window Class

#endif
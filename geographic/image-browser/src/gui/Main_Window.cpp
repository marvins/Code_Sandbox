/**
 * @file    Main_Window.cpp
 * @author  Marvin Smith
 * @date    3/2/2018
 */
#include "Main_Window.hpp"

// Qt Libraries
#include <QSplitter>

// ArcGIS Libraries
#include <Map.h>
#include <MapGraphicsView.h>

using namespace Esri::ArcGISRuntime;

/********************************/
/*          Constructor         */
/********************************/
Main_Window::Main_Window( const Geo_Options& options )
 : m_class_name("Main_Window"),
   m_options(options)
{
    Initialize_GUI();

}


/****************************************/
/*          Initialize the GUI          */
/****************************************/
void Main_Window::Initialize_GUI()
{
    // Build the Central Widget
    m_main_widget = new QSplitter(this);


    // Build the Asset-Manager
    m_asset_panel = new Asset_Panel(m_options);
    m_main_widget->addWidget(m_asset_panel);

    // Add the ArcGIS Map
    Initialize_Map();


    setCentralWidget(m_main_widget);
}


/****************************************/
/*          Initialize the Map          */
/****************************************/
void Main_Window::Initialize_Map()
{
    // Create a map using the Imagery with labels basemap
    m_map = new Map(Basemap::imageryWithLabels(this), this);

    // Create a map view, and pass in the map
    m_mapView = new MapGraphicsView(m_map, this);

    m_main_widget->addWidget(m_mapView);
}
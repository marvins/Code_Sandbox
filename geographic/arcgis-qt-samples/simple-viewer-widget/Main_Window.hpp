/**
 * @file    Main_Window.hpp
 * @author  Marvin Smith
 * @date    8/1/2017
 */
#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

// C++ Libraries
#include <string>

// Qt Libraries
#include <QMainWindow>

// ArcGIS Libraries
#include <Map.h>
#include <MapGraphicsView.h>
#include <Basemap.h>
#include <RasterLayer.h>
#include <Raster.h>

// Define our ESRI Items to be used
namespace Esri {
namespace ArcGISRuntime {
    class Map;
    class MapGraphicsView;
    class Basemap;
    class RasterLayer;
  }
}


/**
 * @class Main_Window
 */
class Main_Window : public QMainWindow
{
    Q_OBJECT

    public:

        /**
         * @brief Constructor
         */
        Main_Window( QWidget*  parent = nullptr );


    protected:

        /**
         * @brief Initialize GUI
        */
        void Initialize_GUI();

    private:

        Esri::ArcGISRuntime::Map* m_map = nullptr;
        Esri::ArcGISRuntime::MapGraphicsView* m_mapView = nullptr;
        Esri::ArcGISRuntime::Basemap* m_basemap = nullptr;


}; // End of Main_Window Class

#endif

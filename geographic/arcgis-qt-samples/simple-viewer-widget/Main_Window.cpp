/**
 * @file    Main_Window.cpp
 * @author  Marvin Smith
 * @date    8/1/2017
 */
#include "Main_Window.hpp"

// Qt Libraries
#include <QVBoxLayout>
#include <QWidget>


using namespace Esri::ArcGISRuntime;


/************************************/
/*          Constructor             */
/************************************/
Main_Window::Main_Window( QWidget* parent )
  : QMainWindow(parent)
{
    // Initialize the Interface
    Initialize_GUI();
}


/********************************/
/*      Initialize the GUI      */
/********************************/
void Main_Window::Initialize_GUI()
{
    Raster* raster_obj = new Raster( QString("/home/ms6401/Desktop/emerald-bay.tif"), this );
    
    RasterLayer* m_raster_layer = new RasterLayer( raster_obj, this );

    // Create the basemap
    m_basemap = new Basemap(m_raster_layer, this);

    // Create the map
    m_map = new Map(m_basemap, this);

    // Create the viewer
    m_mapView = new MapGraphicsView( m_map, this );

    // Set the widget
    QVBoxLayout* main_layout = new QVBoxLayout();
    main_layout->addWidget(m_mapView);

    // Set main widget
    QWidget* main_widget = new QWidget(this);
    main_widget->setLayout( main_layout );

    setCentralWidget( main_widget );

}



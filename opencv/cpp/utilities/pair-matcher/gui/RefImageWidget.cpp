/**
 * @file    RefImageWidget.cpp
 * @author  Marvin Smith
 * @date    2/18/2016
 */
#include "RefImageWidget.hpp"

// Pair-Matcher Libraries
#include "../GDAL_Utilities.hpp"

// C++ Libraries
#include <iostream>

/***********************************/
/*          Constructor            */
/***********************************/
RefImageWidget::RefImageWidget( const Options& options,
                                QWidget*       parent )
 : QWidget(parent),
   m_options(options)
{
    // Initialize GeoTransform
    m_geo_transform = new double[6];


    // Load the OpenCV Image
    std::string ref_image_path = options.Get_Config_Parameter("REF_IMAGE_PATH");


    // Load the GDAL Image
    std::cout << "Loading Ref Image: " << ref_image_path << std::endl;
    m_ref_image = Load_Image_GDAL_To_OpenCV( ref_image_path.c_str(), m_geo_transform );
    
    // Build the Graphics Framework
    m_graphics_scene = new QGraphicsScene();
    m_graphics_view  = new QGraphicsView();
    m_graphics_view->setScene(m_graphics_scene);
    m_graphics_view->setFocusPolicy( Qt::NoFocus );

    // Set the rectangle
    m_graphics_scene->setSceneRect( m_graphics_view->rect() );
    m_scene_rect = m_graphics_view->rect();
    
    // Load the Pixmap
    m_ref_image_pmap = QPixmap::fromImage( QImage( m_ref_image.data, 
                                                   m_ref_image.cols,
                                                   m_ref_image.rows,
                                                   m_ref_image.step,
                                                   QImage::Format_RGBA8888),
                                           Qt::AutoColor );


    // Set the view to the image
    m_graphics_scene->addItem( new QGraphicsPixmapItem(m_ref_image_pmap));

    // Create Layout
    m_layout = new QVBoxLayout();
    m_layout->addWidget( m_graphics_view );

    // Set the layout
    setLayout( m_layout );
    
    // Set Size Policy
    setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Minimum);
    
    // Setup Event Filters
    installEventFilter( m_graphics_view );
    setFocusPolicy( Qt::StrongFocus );
}


/********************************/
/*          Destructor          */
/********************************/
RefImageWidget::~RefImageWidget()
{

    delete [] m_geo_transform;
    m_geo_transform = nullptr;
}

/***********************************/
/*          Event Filter           */
/***********************************/
bool RefImageWidget::eventFilter(QObject *obj, QEvent *event)
{
    // Check for Keyboard Events
    if (event->type() == QEvent::KeyPress) 
    {
        // Cast to our keyboard event
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        keyPressEvent( keyEvent );
        qDebug("Ate key press %d", keyEvent->key());
        return true;
        
    } 
    
    // Otherwise, pass on
    else 
    {
        // standard event processing
        return QObject::eventFilter(obj, event);
    }
}


/**************************************/
/*          Keyboard Events           */
/**************************************/
void RefImageWidget::keyPressEvent( QKeyEvent* key_event )
{
    
    std::cout << "Detected Key: " << key_event->key() << std::endl;
    std::cout << "Arrow Keys: " << Qt::Key_Left << ", " << Qt::Key_Right << std::endl;

    // Check if Arrow Keys Were Pressed
    switch( key_event->key() )
    {
        // Left Arrow Key
        case Qt::Key_Left:
            Pan_Image( -10, 0 );
            break;

        case Qt::Key_Right:
            Pan_Image( 10, 0 );
            break;

        case Qt::Key_Up:
            Pan_Image( 0, -10 );
            break;

        case Qt::Key_Down:
            Pan_Image( 0, 10 );
            break;
    
        // Zoom In
        case Qt::Key_BracketLeft:
            Zoom_Image( 0.9 );
            break;
        case Qt::Key_BracketRight:
            Zoom_Image( 1.1 );
            break;
    }
}


/*****************************************/
/*          Translate the Image          */
/*****************************************/
void RefImageWidget::Pan_Image( const double& x,
                                 const double& y )
{

    // Update the scene rectangle
    m_scene_rect.setX( m_scene_rect.x() + x);
    m_scene_rect.setY( m_scene_rect.y() + y);

    m_graphics_view->setSceneRect( m_scene_rect );
}


/********************************************/
/*              Scale the Image             */
/********************************************/
void RefImageWidget::Zoom_Image( const double& scale )
{
    m_graphics_view->scale( scale, scale );

}



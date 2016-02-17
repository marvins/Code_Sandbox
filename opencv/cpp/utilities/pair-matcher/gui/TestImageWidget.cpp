/**
 * @file    TestImageWidget.cpp
 * @author  Marvin Smith
 * @date    2/17/2016
*/
#include "TestImageWidget.hpp"

// C++ Libraries
#include <cinttypes>
#include <iostream>
#include <string>


/***********************************/
/*          Constructor            */
/***********************************/
TestImageWidget::TestImageWidget( Options const& options, 
                                  QWidget*       parent )
 : QWidget(parent),
   m_options(options)
{
    
    // Load the OpenCV Image
    std::string test_image_path = options.Get_Config_Parameter("TEST_IMAGE_PATH");
    m_test_image = cv::imread( test_image_path.c_str() );
    cv::cvtColor( m_test_image, m_test_image, CV_BGR2RGB); 


    // Build the Graphics Framework
    m_graphics_scene = new QGraphicsScene();
    m_graphics_view  = new QGraphicsView();
    m_graphics_view->setScene(m_graphics_scene);
    m_graphics_view->setFocusPolicy( Qt::NoFocus );

    // Set the rectangle
    m_graphics_scene->setSceneRect( m_graphics_view->rect() );
    m_scene_rect = m_graphics_view->rect();
    
    // Load the Pixmap
    m_test_image_pmap = QPixmap::fromImage( QImage( m_test_image.data, 
                                                    m_test_image.cols,
                                                    m_test_image.rows,
                                                    m_test_image.step,
                                                    QImage::Format_RGB888),
                                            Qt::AutoColor );


    // Set the view to the image
    m_graphics_scene->addItem( new QGraphicsPixmapItem(m_test_image_pmap));

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


/***********************************/
/*          Event Filter           */
/***********************************/
bool TestImageWidget::eventFilter(QObject *obj, QEvent *event)
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
void TestImageWidget::keyPressEvent( QKeyEvent* key_event )
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
void TestImageWidget::Pan_Image( const double& x,
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
void TestImageWidget::Zoom_Image( const double& scale )
{
    m_graphics_view->scale( scale, scale );

}



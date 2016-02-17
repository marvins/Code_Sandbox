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


    // Set the rectangle
    m_graphics_scene->setSceneRect( m_graphics_view->rect() );
    m_graphics_view->setFixedSize( 500, 500 );
    
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
    
}



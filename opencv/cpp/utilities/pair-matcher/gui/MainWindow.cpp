/**
 * @file    MainWindow.cpp
 * @author  Marvin Smith
 * @date    2/17/2016
*/
#include "MainWindow.hpp"

// C++ Libraries
#include <iostream>


/**********************************/
/*          Constructor           */
/**********************************/
MainWindow::MainWindow( Options const& options )
 : m_class_name("MainWindow"),
   m_options(options)
{
    // Set Window Title
    setWindowTitle("Pair Matcher");

    // Set the Window Size
    resize(1000,500);
    
    // Build Docket Windows
    Build_Dock_Windows();
    
    // Define a strong focus 
    setFocusPolicy ( Qt::StrongFocus );
}


/***************************************/
/*          Build Dock Windows         */
/***************************************/
void MainWindow::Build_Dock_Windows()
{
    // Create Reference Image Dock Widget
    m_ref_image_dock = new QDockWidget(tr("Reference Imagery"), this);
    
    addDockWidget( Qt::LeftDockWidgetArea, m_ref_image_dock );


    // Create Test Image Dock Widget
    m_tst_image_dock = new QDockWidget(tr("Test Imagery"), this);

    // Load the test image
    m_tst_image_dock->setWidget( new TestImageWidget( m_options, this )); 
    addDockWidget( Qt::RightDockWidgetArea, m_tst_image_dock );
}


/************************************/
/*          Keyboard Event          */
/************************************/
void MainWindow::keyPressEvent( QKeyEvent* event )
{
    std::cout << m_class_name << "::keyPressEvent, Keyboard detected: " << event->key() << std::endl;
}



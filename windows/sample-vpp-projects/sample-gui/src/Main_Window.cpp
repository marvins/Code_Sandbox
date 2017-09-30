/**
 * @file    Main_Window.cpp
 * @author  Marvin Smith
 * @date    8/4/2017
 */
#include "Main_Window.hpp"

// C++ Libraries
#include <iostream>

// Boost Libraries
#include <boost/filesystem.hpp>

// Qt Libraries
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>


/**********************************/
/*          Constructor           */
/**********************************/
Main_Window::Main_Window( QWidget* parent )
        : QMainWindow(parent),
          m_class_name("Main_Window")
{
    // Initialize the GUI
    Initialize_GUI();
}


/****************************************/
/*          Inititalize the GUI         */
/****************************************/
void Main_Window::Initialize_GUI()
{
    // Create the stack Widgets
    m_stack_widget = new QStackedWidget(this);

	// Check if Path Exists
	if (boost::filesystem::exists(boost::filesystem::path("C:\\Qt")))
	{
		std::cout << "Found QT Base Directory" << std::endl;
	}
	else
	{
		std::cout << "Did not find QT Base Repository" << std::endl;
	}

    // Add widget to main gui
    setCentralWidget( m_stack_widget );

    // Build the Menu Bar
    Build_Menu();

}


/****************************/
/*        Build Menu        */
/****************************/
void Main_Window::Build_Menu()
{
    //  Create a File Menu
    QMenu* fileMenu = new QMenu(tr("File"));
    QMenu* projectMenu = new QMenu(tr("Project"));

    // Add Quit Action
    QAction* quitAction = new QAction(tr("&Quit"), this);
    quitAction->setShortcuts(QKeySequence::Quit);
    quitAction->setStatusTip("Quit Program");
    connect( quitAction, SIGNAL(triggered()), this, SLOT(close()));
    fileMenu->addAction(quitAction);

    // Add file menu
    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(projectMenu);

}

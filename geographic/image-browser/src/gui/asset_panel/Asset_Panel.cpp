/**
 * @file    Asset_Panel.cpp
 * @author  Marvin Smith
 * @date    3/2/2018
 */
#include "Asset_Panel.hpp"

// Qt Libraries
#include <QGroupBox>

/********************************/
/*          Constructor         */
/********************************/
Asset_Panel::Asset_Panel(const Geo_Options& options,
                         QWidget*           parent )
  : QWidget(parent),
    m_class_name("Asset_Panel"),
    m_options(options)
{
    Initialize_GUI();

    setObjectName("Asset View");
}


/****************************************/
/*          Initialize the GUI          */
/****************************************/
void Asset_Panel::Initialize_GUI()
{
    m_main_layout = new QVBoxLayout();
    setLayout(m_main_layout);


    // Create the toolbar
    Build_Toolbar();
}


/****************************************/
/*          Build the Toolbar           */
/****************************************/
void Asset_Panel::Build_Toolbar()
{
    // Create Widget
    QGroupBox* toolbar_widget = new QGroupBox("Toolbar",this);

    // Add to layout
    m_main_layout->addWidget(toolbar_widget);
}

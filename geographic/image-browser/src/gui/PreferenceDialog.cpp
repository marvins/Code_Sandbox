/**
 * @file     PreferenceDialog.cpp
 * @author   Marvin Smith
 * @date     10/1/2013
 */
#include "PreferenceDialog.hpp"


/**
 *  Default Constructor
 */
PreferenceDialog::PreferenceDialog( QWidget* parent ) : QDialog(parent){

    /// create main layout
    mainLayout = new QVBoxLayout;

    // create main label
    mainLabel = new QLabel("Preferences");
    mainLayout->addWidget( mainLabel );

    // set main layout
    setLayout( mainLayout );

}



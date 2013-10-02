/**
 * @file   IndexingProgressDialog.cpp
 * @author Marvin Smith
 * @date   10/2/2013
*/
#include <src/gui/IndexingProgressDialog.hpp>

#include <iostream>

using namespace std;

IndexingProgressDialog::IndexingProgressDialog( QWidget* parent ) : QDialog(parent){
    
    // create the main layout
    mainLayout = new QVBoxLayout;
    mainLayout->setAlignment( Qt::AlignTop );

    // create the main label
    mainLabel = new QLabel("Indexing Filesystem");
    mainLayout->addWidget( mainLabel );

    // create the progress bar
    progressBar = new QProgressBar(this);
    progressBar->setMinimum(0);
    progressBar->setMaximum(100);
    progressBar->setValue(0);
    mainLayout->addWidget(progressBar);
    
    // create the status bar
    statusLabel = new QLabel("Starting Index");
    statusLabel->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
    mainLayout->addWidget( statusLabel );


    /// Create the button bar
    buttonBar = new QWidget;

    // create the layout
    buttonBarLayout = new QHBoxLayout;
    buttonBarLayout->setAlignment( Qt::AlignLeft );

    // create the button
    buttonBarCancelButton = new QToolButton;
    buttonBarCancelButton->setText("Cancel");
    buttonBarCancelButton->setFixedWidth(80);
    buttonBarCancelButton->setFixedHeight(40);
    buttonBarLayout->addWidget( buttonBarCancelButton );


    // set the layout
    buttonBar->setLayout( buttonBarLayout );

    // add to main widget
    mainLayout->addWidget( buttonBar );

    // set the main layout
    setLayout( mainLayout );

    // set some flags
    setWindowFlags( Qt::WindowStaysOnTopHint | Qt::WindowTitleHint | Qt::CustomizeWindowHint );


    // set resize policy
    setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
    
    // set window modality so it sits on top
    setWindowModality( Qt::WindowModal );

}


void IndexingProgressDialog::updateValue( const int& value ){
    
    cout << "Update Value" << endl;
    progressBar->setValue(value);
}

void IndexingProgressDialog::updateStatus( const string& value ){

    cout << "Update Status" << endl;
    statusLabel->setText(value.c_str());
    adjustSize();
}



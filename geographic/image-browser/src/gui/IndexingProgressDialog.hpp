/**
 * @file   IndexingProgressDialog.hpp
 * @author  Marvin Smith
 * @date    10/2/2013
*/
#ifndef __SRC_GUI_INDEXINGPROGRESSDIALOG_HPP__
#define __SRC_GUI_INDEXINGPROGRESSDIALOG_HPP__

#include <string>

#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWidget>

#include <src/core/MessagingService.hpp>

extern MessagingService message_service;

/**
 * @class IndexingProgressDialog
*/
class IndexingProgressDialog : public QDialog{

    Q_OBJECT

    signals:
        
        void cancelIndexSignal();

    public:

        /**
         * Default Constructor
        */
        IndexingProgressDialog( QWidget* parent = NULL );
       
    public slots:

        /**
         * Update Value
        */
        void updateValue( const int& value );
    
        /**
         * Update Label
        */
        void updateStatus( const std::string& value );

    private:
        
        /// Main Layout
        QVBoxLayout* mainLayout;
        
        /// Main Label
        QLabel*      mainLabel;
        
        /// Main Progress Bar
        QProgressBar*  progressBar;
        
        /// Status label
        QLabel*   statusLabel;

        /// Button Bar
        QWidget* buttonBar;

        /// Button Layout
        QHBoxLayout*  buttonBarLayout;

        /// Close button
        QToolButton*  buttonBarCancelButton;

};

#endif


/**
 * @file   BrowserPane.hpp
 * @author Marvin Smith
 * @date   10/1/2013
 */
#ifndef __SRC_GUI_BROWSERPANE_HPP__
#define __SRC_GUI_BROWSERPANE_HPP__

#include <QGridLayout>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWebView>
#include <QWidget>

class BrowserPane : public QWidget{

    Q_OBJECT

    public:

        /**
         * Default Constructor
         */
        BrowserPane( QWidget* parent = NULL );
    
        /**
         *  Set the url of the browser pane
         */
        void setUrl( std::string const& url );

    private:
        
        //-----------------------------//
        //-     Private Functions     -//
        //-----------------------------//
        void build_toolbar_widget();

        //-----------------------------//
        //-     Private Variables     -//
        //-----------------------------//
        
        /// Main Layout
        QGridLayout*  mainLayout;

        ///  Web View
        QWebView*  webView;
        
        /// Toolbar Widget
        QWidget*      toolbarWidget;
        
        /// Toolbar Layout
        QVBoxLayout*  toolbarLayout;

        /// Toolbar Look At Button
        QToolButton*  toolbarLookAtButton;

        /// Toolbar Export Button
        QToolButton*  toolbarExportButton;

};

#endif


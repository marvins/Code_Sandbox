/**
 * @file   MainWindow.hpp
 * @author Marvin Smith
 * @date   10/1/2013
 */
#ifndef __SRC_GUI_MAINWINDOW_HPP__
#define __SRC_GUI_MAINWINDOW_HPP__

#include <QMainWindow>
#include <QWebView>
#include <QWidget>

#include <src/gui/AssetPane.hpp>

/**
 * @class MainWindow
 */
class MainWindow : public QMainWindow{

    Q_OBJECT

    public:
        
        MainWindow( );


    private:

        /// Main Window Splitter
        QSplitter* splitter;

        /// Asset Panel
        AssetPane* assetPane;

        /// Web View
        QWebView*  webView;


};

#endif

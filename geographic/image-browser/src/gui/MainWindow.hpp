/**
 * @file   MainWindow.hpp
 * @author Marvin Smith
 * @date   10/1/2013
 */
#ifndef __SRC_GUI_MAINWINDOW_HPP__
#define __SRC_GUI_MAINWINDOW_HPP__

#include <QAction>
#include <QMainWindow>
#include <QMenuBar>
#include <QSplitter>
#include <QWebView>
#include <QWidget>

#include <src/gui/AssetPane.hpp>
#include <src/gui/BrowserPane.hpp>
#include <src/gui/PreferenceDialog.hpp>

#include <string>

/**
 * @class MainWindow
 */
class MainWindow : public QMainWindow{

    Q_OBJECT

    public:
        
        /**
         * Default Constructor
         */
        MainWindow( );

    public slots:
        
        void loadPreferencePane();

    private:
        
        /// Build Menu
        void build_menu();

        /// Main Window Splitter
        QSplitter* splitter;
        
        /// Asset Panel
        AssetPane*   assetPane;

        /// Browser Panel
        BrowserPane* browserPane;
        
        /// File Menu
        QMenu*  fileMenu;

        /// Quit Action
        QAction* quitAction;

        /// Options Menu
        QMenu*  optionsMenu;
        
        /// Preference Pane Action
        QAction*  prefAction;

};

#endif

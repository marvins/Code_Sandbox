/**
 * @file    AssetPane.hpp
 * @author  Marvin Smith
 * @date    10/1/2013
 */
#ifndef  __SRC_GUI_ASSETPANE_HPP__
#define  __SRC_GUI_ASSETPANE_HPP__

#include <QLabel>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QWidget>

#include <src/core/DataContainer.hpp>
extern DataContainer settings;

/**
 * @class AssetPane
 */
class AssetPane : public QWidget{

    Q_OBJECT

    public:
        
        /**
         * Default Constructor
         */
        AssetPane( QWidget* parent = NULL );
        
    public slots:

        void filesystemClicked();

    private:
        
        //-------------------------------//
        //-      Private Functions      -//
        //-------------------------------//
        
        /**
         *  Build the filesystem viewing widget
         */
        void build_filesystem_widget();
        
        /**
         *  Build the asset list widget
         */
        void build_asset_widget();

        void build_directory_tree();

        //-------------------------------//
        //-      Private Variables      -//
        //-------------------------------//

        /// Primary layout
        QVBoxLayout*  mainLayout;

        /// Filesystem widget
        QWidget*      filesystemWidget;
        
        /// Filesystem Layout
        QVBoxLayout*  filesystemLayout;
        
        /// Filesystem Label
        QLabel*       filesystemLabel;
        QLabel*       filesystemPath;
        
        /// Filesystem Tree Widget
        QTreeWidget*      filesystemTree;
        QTreeWidgetItem*  filesystemHeaderItem;


        /// Asset Widget
        QWidget*      assetWidget;

        /// Asset Layout
        QVBoxLayout*  assetLayout;

        /// Asset Label
        QLabel*       assetLabel;

        /// QTreeView
        QTreeWidget*      assetTree;
        QTreeWidgetItem*  assetHeaderItem;

}; 

#endif


/**
 * @file    AssetPane.hpp
 * @author  Marvin Smith
 * @date    10/1/2013
 */
#ifndef  __SRC_GUI_ASSETPANE_HPP__
#define  __SRC_GUI_ASSETPANE_HPP__

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QToolButton>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QWidget>

#include <src/core/DataContainer.hpp>
#include <src/core/GDALLoader.hpp>
#include <src/core/MessagingService.hpp>

#include <src/gui/IndexingWorker.hpp>

extern DataContainer     settings;
extern MessagingService  message_service;

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

        void indexFilesystem();
        
        void showOverlayClicked();

        void assetSelected();
        
        void reloadAssetTree();

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

        /**
         * Create the list of directories to search
        */
        void build_directory_tree();
        
        
        /**
         * Build the button bar on the asset widget
        */
        void build_asset_button_bar();

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
        QLineEdit*       filesystemPath;
        
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

        // asset button bar
        bool showOverlay;
        QWidget*      assetButtonBarWidget;
        QHBoxLayout*  assetButtonBarLayout;
        QToolButton*  assetSearchButton;
        QToolButton*  assetShowOverlayButton;


}; 

#endif


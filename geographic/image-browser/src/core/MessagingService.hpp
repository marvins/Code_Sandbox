/**
 * @file    MessagingService.hpp
 * @author  Marvin Smith
 * @date    10/2/2013
*/
#ifndef __SRC_CORE_MESSAGING_SERVICE_HPP__
#define __SRC_CORE_MESSAGING_SERVICE_HPP__

#include <QtGui>
#include <QWidget>

/**
 * @class MessagingService
*/
class MessagingService : public QObject{

    Q_OBJECT

    signals:
        
        /**
         * Signal to reload all browser overlays
        */
        void reloadBrowserOverlaySignal();

        /**
         * Signal telling the asset pane to refresh the asset tree
        */
        void reloadAssetTreeSignal();

        /**
         * Signal telling the indexing progress dialog to show
        */
        void showIndexingProgressDialogSignal();

        void closeIndexingProgressDialogSignal();
        
        void updateIndexingProgressDialogStatusSignal();

};

#endif

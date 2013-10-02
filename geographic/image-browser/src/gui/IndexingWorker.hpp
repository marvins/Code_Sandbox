/**
 * @file   IndexingWorker.hpp
 * @author Marvin Smith
 * @date   10/2/2013
*/
#ifndef __SRC_GUI_INDEXINGWORKER_HPP__
#define __SRC_GUI_INDEXINGWORKER_HPP__

#include <QtGui>
#include <QWidget>

#include <src/core/DataContainer.hpp>
#include <src/core/MessagingService.hpp>

extern DataContainer settings;
extern MessagingService message_service;


/**
 * @class IndexingWorker
*/
class IndexingWorker : public QObject {

    Q_OBJECT

    public slots:
        
        /**
         * Function to index the filesystem for assets
        */
        void startIndexing();
        
};



#endif

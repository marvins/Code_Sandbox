/**
 * @file    MessagingService.cpp
 * @author  Marvin Smith
 * @date    10/2/2013
*/
#include <src/core/MessagingService.hpp>

/**
 * Slot to call reload function
*/
void MessagingService::reloadBrowserOverlaySlot(){
    
    emit reloadBrowserOverlaySignal();

}

void MessagingService::reloadAssetTreeSlot( ){
    emit reloadAssetTreeSignal();
}

void MessagingService::showIndexingProgressDialogSlot(){
    emit showIndexingProgressDialogSignal();
}

void MessagingService::closeIndexingProgressDialogSlot(){
    emit closeIndexingProgressDialogSignal();
}

void MessagingService::updateIndexingProgressDialogStatusSlot(){
    emit updateIndexingProgressDialogStatusSignal();
}


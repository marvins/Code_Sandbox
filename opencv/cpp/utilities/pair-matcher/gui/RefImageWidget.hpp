/**
 * @file    RefImageWidget.hpp
 * @author  Marvin Smith
 * @date    2/18/2016
 */
#ifndef __PAIR_MATCHER_REF_IMAGE_WIDGET_HPP__
#define __PAIR_MATCHER_REF_IMAGE_WIDGET_HPP__

// Pair-Matcher Libraries
#include "../Options.hpp"

// Qt Libraries
#include <QtGui>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QGridLayout>
#include <QPixmap>
#include <QVBoxLayout>
#include <QWidget>


// OpenCV Libraries
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


// C++ Libraries
#include <string>

/**
 * @class RefImageWidget
 */
class RefImageWidget : public QWidget
{
    Q_OBJECT

    public:
        
        /**
         * @brief Constructor
         */
        RefImageWidget( Options const& options,
                        QWidget*       parent );

        
        /**
         * @brief Destructor
         */
        virtual ~RefImageWidget();

    
    protected:
        
        /**
         * @brief Translate the Image
        */
        void Pan_Image( const double& x,
                              const double& y );

        /**
         * @brief Scale the Image
        */
        void Zoom_Image( const double& scale );


        /**
         * @brief Detect Keyboard Actions.
        */
        void keyPressEvent( QKeyEvent* key_event ); 
        
        /**
         * @brief Event Filter
        */
        bool eventFilter(QObject *obj, QEvent *event);

    private:
        
        /// Configuration Options
        Options m_options;

        /// Image Data
        cv::Mat m_ref_image;
        QPixmap m_ref_image_pmap;

        /// Graphics View
        QGraphicsView* m_graphics_view;

        /// Scene
        QGraphicsScene* m_graphics_scene;

        /// GeoTransform
        double* m_geo_transform;
        
        /// Layout
        QVBoxLayout* m_layout;

        /// Scene Rectangle
        QRectF m_scene_rect;


}; // End of RefImageWidget Class


#endif

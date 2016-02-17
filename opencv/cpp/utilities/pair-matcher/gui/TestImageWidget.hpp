/**
 * @file    TestImageWidget.hpp
 * @author  Marvin Smith
 * @date    2/17/2016
*/
#ifndef __PAIR_MATCHER_TEST_IMAGE_WIDGET_HPP__
#define __PAIR_MATCHER_TEST_IMAGE_WIDGET_HPP__

// Pair Matcher Libraries
#include "../Options.hpp"

// Qt Libraries
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


/**
 * @class TestImageWidget
*/
class TestImageWidget : public QWidget
{

    public:
        
        /**
         * @brief Constructor
        */
        TestImageWidget( Options const& options,
                         QWidget*       parent );

    private:
        
        /// Configuration Options
        Options m_options;

        /// Image Data
        cv::Mat m_test_image;
        QPixmap m_test_image_pmap;

        /// Graphics View
        QGraphicsView* m_graphics_view;

        /// Scene
        QGraphicsScene* m_graphics_scene;

        
        /// Layout
        QVBoxLayout* m_layout;

}; // End of Options Class

#endif

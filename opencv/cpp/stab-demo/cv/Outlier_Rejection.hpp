#ifndef STAB_DEMO_CV_VIDEOSTAB_OUTLIER_REJECTION_HPP
#define STAB_DEMO_CV_VIDEOSTAB_OUTLIER_REJECTION_HPP

#include <vector>

#include <opencv2/core.hpp>

// Project Libraries
#include "Motion_Core.hpp"


class IOutlierRejector
{
    public:

        virtual ~IOutlierRejector() = default;

        virtual void process( cv::Size        frameSize,
                              cv::InputArray  points0,
                              cv::InputArray  points1,
                              cv::OutputArray mask) = 0;
};

class NullOutlierRejector : public IOutlierRejector
{
    public:

        void process( cv::Size        frameSize,
                      cv::InputArray  points0,
                      cv::InputArray  points1,
                      cv::OutputArray mask) override;
};

class TranslationBasedLocalOutlierRejector : public IOutlierRejector
{
    public:

        TranslationBasedLocalOutlierRejector();

        void setCellSize( cv::Size val )
        {
            cellSize_ = val;
        }

        cv::Size cellSize() const
        {
            return cellSize_;
        }

        void setRansacParams(RansacParams val)
        {
            ransacParams_ = val;
        }

        RansacParams ransacParams() const
        {
            return ransacParams_;
        }

        virtual void process( cv::Size frameSize,
                              cv::InputArray points0,
                              cv::InputArray points1,
                              cv::OutputArray mask ) override;

    private:

        cv::Size cellSize_;
        RansacParams ransacParams_;

        typedef std::vector<int> Cell;
        std::vector<Cell> grid_;
};


#endif
#ifndef STAB_DEMO_CV_VIDEOSTAB_OPTICAL_FLOW_HPP
#define STAB_DEMO_CV_VIDEOSTAB_OPTICAL_FLOW_HPP

// OpenCV Libraries
#include <opencv2/core.hpp>

class ISparseOptFlowEstimator
{
    public:
        virtual ~ISparseOptFlowEstimator() = default;

        virtual void run( cv::InputArray frame0,
                          cv::InputArray frame1,
                          cv::InputArray points0,
                          cv::InputOutputArray points1,
                          cv::OutputArray status,
                          cv::OutputArray errors) = 0;
};

class IDenseOptFlowEstimator
{
public:

    virtual ~IDenseOptFlowEstimator() = default;

    virtual void run( cv::InputArray frame0,
                      cv::InputArray frame1,
                      cv::InputOutputArray flowX,
                      cv::InputOutputArray flowY,
                      cv::OutputArray errors) = 0;
};

class PyrLkOptFlowEstimatorBase
{
    public:

        PyrLkOptFlowEstimatorBase()
        {
            setWinSize(cv::Size(21, 21));
            setMaxLevel(3);
        }

        virtual void setWinSize( cv::Size val)
        {
            winSize_ = val;
        }

        virtual cv::Size winSize() const
        {
            return winSize_;
        }

        virtual void setMaxLevel(int val)
        {
            maxLevel_ = val;
        }

        virtual int maxLevel() const
        {
            return maxLevel_;
        }

        virtual ~PyrLkOptFlowEstimatorBase() = default;

    protected:

        cv::Size winSize_;
        int maxLevel_ { -1 };
};


class SparsePyrLkOptFlowEstimator : public PyrLkOptFlowEstimatorBase, public ISparseOptFlowEstimator
{
    public:

        void run( cv::InputArray frame0,
                  cv::InputArray frame1,
                  cv::InputArray points0,
                  cv::InputOutputArray points1,
                  cv::OutputArray status,
                  cv::OutputArray errors) override;
};



#endif

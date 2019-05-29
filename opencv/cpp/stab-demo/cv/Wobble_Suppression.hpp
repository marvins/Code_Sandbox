#ifndef STAB_DEMO_CV_VIDEOSTAB_WOBBLE_SUPPRESSION_HPP
#define STAB_DEMO_CV_VIDEOSTAB_WOBBLE_SUPPRESSION_HPP

// C++ Libraries
#include <memory>
#include <vector>

// OpenCV Libraries
#include <opencv2/core.hpp>

// Project Libraries
#include "Global_Motion.hpp"
#include "Log.hpp"


class WobbleSuppressorBase
{
    public:
        WobbleSuppressorBase();

        virtual ~WobbleSuppressorBase() = default;

        void setMotionEstimator( cv::Ptr<ImageMotionEstimatorBase> val)
        {
            motionEstimator_ = val;
        }

        cv::Ptr<ImageMotionEstimatorBase> motionEstimator() const
        {
            return motionEstimator_;
        }

        virtual void suppress(int idx, const cv::Mat &frame, cv::Mat &result) = 0;


        // data from stabilizer

        virtual void setFrameCount(int val)
        {
            frameCount_ = val;
        }

        virtual int frameCount() const
        {
            return frameCount_;
        }

        virtual void setMotions(const std::vector<cv::Mat> &val)
        {
            motions_ = &val;
        }

        virtual const std::vector<cv::Mat> &motions() const
        {
            return *motions_;
        }

        virtual void setMotions2(const std::vector<cv::Mat> &val)
        {
            motions2_ = &val;
        }

        virtual const std::vector<cv::Mat> &motions2() const
        {
            return *motions2_;
        }

        virtual void setStabilizationMotions(const std::vector<cv::Mat> &val)
        {
            stabilizationMotions_ = &val;
        }

        virtual const std::vector<cv::Mat> &stabilizationMotions() const
        {
            return *stabilizationMotions_;
        }

    protected:
        cv::Ptr<ImageMotionEstimatorBase> motionEstimator_;
        int frameCount_;
        const std::vector<cv::Mat> *motions_;
        const std::vector<cv::Mat> *motions2_;
        const std::vector<cv::Mat> *stabilizationMotions_;
};

class NullWobbleSuppressor : public WobbleSuppressorBase
{
    public:
        void suppress(int idx, const cv::Mat &frame, cv::Mat &result) override;
};

class MoreAccurateMotionWobbleSuppressorBase : public WobbleSuppressorBase
{
    public:
        virtual void setPeriod(int val)
        { period_ = val; }

        virtual int period() const
        { return period_; }

    protected:

        MoreAccurateMotionWobbleSuppressorBase()
        {
            setPeriod(30);
        }

        int period_ { -1 };
};

class MoreAccurateMotionWobbleSuppressor : public MoreAccurateMotionWobbleSuppressorBase
{
    public:
        virtual void suppress(int idx, const cv::Mat &frame, cv::Mat &result) override;

    private:
        cv::Mat_<float> mapx_, mapy_;
};


#endif

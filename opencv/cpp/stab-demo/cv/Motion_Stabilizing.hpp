#ifndef STAB_DEMO_CV_VIDEOSTAB_MOTION_STABILIZING_HPP
#define STAB_DEMO_CV_VIDEOSTAB_MOTION_STABILIZING_HPP

// C++ Libraries
#include <vector>
#include <utility>

// OpenCV Libraries
#include <opencv2/core.hpp>

// Project Libraries
#include "Global_Motion.hpp"


class IMotionStabilizer
{
    public:

        virtual ~IMotionStabilizer() = default;

        //! assumes that [0, size-1) is in or equals to [range.first, range.second)
        virtual void stabilize( int size,
                                const std::vector<cv::Mat>& motions,
                                std::pair<int, int> range,
                                cv::Mat *stabilizationMotions) = 0;
};


class MotionStabilizationPipeline : public IMotionStabilizer
{
    public:

        void pushBack(std::shared_ptr<IMotionStabilizer> stabilizer)
        {
            stabilizers_.push_back(stabilizer);
        }

        bool empty() const
        {
            return stabilizers_.empty();
        }

        void stabilize( int size,
                        const std::vector<cv::Mat>& motions,
                        std::pair<int, int> range,
                        cv::Mat *stabilizationMotions) override;

    private:

        std::vector<std::shared_ptr<IMotionStabilizer>> stabilizers_;
};


class MotionFilterBase : public IMotionStabilizer
{
    public:

        ~MotionFilterBase() override = default;

        virtual cv::Mat stabilize( int idx,
                                   const std::vector<cv::Mat>& motions,
                                   std::pair<int, int> range) = 0;

        void stabilize( int size,
                        const std::vector<cv::Mat>& motions,
                        std::pair<int, int> range,
                        cv::Mat *stabilizationMotions) override;
};

class GaussianMotionFilter : public MotionFilterBase
{
    public:

        GaussianMotionFilter(int radius = 15, float stdev = -1.f)
        {
            setParams( radius, stdev);
        }

        void setParams(int radius, float stdev = -1.f);

        int radius() const
        {
            return radius_;
        }

        float stdev() const
        {
            return stdev_;
        }

        virtual cv::Mat stabilize( int idx,
                                   const std::vector<cv::Mat>& motions,
                                   std::pair<int, int> range) override;

    private:
        int radius_;
        float stdev_;
        std::vector<float> weight_;
};


class LpMotionStabilizer : public IMotionStabilizer
{
    public:

        LpMotionStabilizer(MotionModel model = MM_SIMILARITY);

        void setMotionModel(MotionModel val)
        {
            model_ = val;
        }

        MotionModel motionModel() const
        {
            return model_;
        }

        void setFrameSize( cv::Size val)
        {
            frameSize_ = val;
        }

        cv::Size frameSize() const
        {
            return frameSize_;
        }

        void setTrimRatio(float val)
        {
            trimRatio_ = val;
        }

        float trimRatio() const
        {
            return trimRatio_;
        }

        void setWeight1(float val)
        {
            w1_ = val;
        }

        float weight1() const
        {
            return w1_;
        }

        void setWeight2(float val)
        {
            w2_ = val;
        }

        float weight2() const
        {
            return w2_;
        }

        void setWeight3(float val)
        {
            w3_ = val;
        }

        float weight3() const
        {
            return w3_;
        }

        void setWeight4(float val)
        {
            w4_ = val;
        }

        float weight4() const
        {
            return w4_;
        }


        virtual void stabilize( int size,
                                const std::vector<cv::Mat>& motions,
                                std::pair<int, int> range,
                                cv::Mat *stabilizationMotions ) override;

    private:
        MotionModel model_;
        cv::Size frameSize_;
        float trimRatio_;
        float w1_, w2_, w3_, w4_;

        std::vector<double> obj_, collb_, colub_;
        std::vector<int> rows_, cols_;
        std::vector<double> elems_, rowlb_, rowub_;

        void set(int row, int col, double coef)
        {
            rows_.push_back(row);
            cols_.push_back(col);
            elems_.push_back(coef);
        }
};


cv::Mat ensureInclusionConstraint( const cv::Mat& M, cv::Size size, float trimRatio);

float estimateOptimalTrimRatio( const cv::Mat& M, cv::Size size);



#endif

#ifndef STAB_DEMO_CV_VIDEOSTAB_GLOBAL_MOTION_HPP
#define STAB_DEMO_CV_VIDEOSTAB_GLOBAL_MOTION_HPP

// C++ Libraries
#include <fstream>
#include <memory>
#include <vector>

// OpenCV Libraries
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>

// Project Libraries
#include "Optical_Flow.hpp"
#include "Motion_Core.hpp"
#include "Outlier_Rejection.hpp"


/** @brief Estimates best global motion between two 2D point clouds in the least-squares sense.
 *
 * @note Works in-place and changes input point arrays.
 *
 * @param points0 Source set of 2D points (32F).
 * @param points1 Destination set of 2D points (32F).
 * @param model Motion model (up to MM_AFFINE).
 * @param rmse Final root-mean-square error.
 * @return 3x3 2D transformation matrix (32F).
 */
cv::Mat estimateGlobalMotionLeastSquares( cv::InputOutputArray points0,
                                          cv::InputOutputArray points1,
                                          int model = MM_AFFINE,
                                          float *rmse = 0);

/** @brief Estimates best global motion between two 2D point clouds robustly (using RANSAC method).
 *
 * @param points0 Source set of 2D points (32F).
 * @param points1 Destination set of 2D points (32F).
 * @param model Motion model. See cv::videostab::MotionModel.
 * @param params RANSAC method parameters. See RansacParams.
 * @param rmse Final root-mean-square error.
 * @param ninliers Final number of inliers.
 */
cv::Mat estimateGlobalMotionRansac( cv::InputArray points0,
                                    cv::InputArray points1,
                                    int model = MM_AFFINE,
                                    const RansacParams &params = RansacParams::default2dMotion(MM_AFFINE),
                                    float *rmse = 0,
                                    int *ninliers = 0);


/** @brief Base class for all global motion estimation methods.
 */
class MotionEstimatorBase
{
    public:

        virtual ~MotionEstimatorBase() = default;

        /**
         * @brief Sets motion model.
         * @param val Motion model. See cv::videostab::MotionModel.
         */
        virtual void setMotionModel(MotionModel val)
        {
            motionModel_ = val;
        }

        /**
         * @return Motion model. See cv::videostab::MotionModel.
        */
        virtual MotionModel motionModel() const{ return motionModel_; }


        /** @brief Estimates global motion between two 2D point clouds.
         *
         * @param points0 Source set of 2D points (32F).
         * @param points1 Destination set of 2D points (32F).
         * @param ok Indicates whether motion was estimated successfully.
         * @return 3x3 2D transformation matrix (32F).
         */
        virtual cv::Mat estimate( cv::InputArray points0,
                                  cv::InputArray points1,
                                  bool*          ok = 0) = 0;

    protected:

        MotionEstimatorBase(MotionModel model)
        {
            setMotionModel(model);
        }

    private:

        MotionModel motionModel_;
};

/** @brief Describes a robust RANSAC-based global 2D motion estimation method which minimizes L2 error.
 */
class MotionEstimatorRansacL2 : public MotionEstimatorBase
{
    public:

        MotionEstimatorRansacL2(MotionModel model = MM_AFFINE);

        void setRansacParams(const RansacParams &val)
        {
            ransacParams_ = val;
        }

        RansacParams ransacParams() const
        {
            return ransacParams_;
        }

        void setMinInlierRatio(float val)
        {
            minInlierRatio_ = val;
        }

        float minInlierRatio() const{
            return minInlierRatio_;
        }

        virtual cv::Mat estimate( cv::InputArray points0,
                                  cv::InputArray points1,
                                  bool *ok = 0) override;

    private:
        RansacParams ransacParams_;
        float minInlierRatio_;
};

/** @brief Describes a global 2D motion estimation method which minimizes L1 error.
 *
 * @note To be able to use this method you must build OpenCV with CLP library support. :
 */
class MotionEstimatorL1 : public MotionEstimatorBase
{
    public:
        explicit MotionEstimatorL1(MotionModel model = MM_AFFINE);

        cv::Mat estimate( cv::InputArray points0,
                          cv::InputArray points1,
                          bool *ok = 0) override;

    private:
        std::vector<double> obj_, collb_, colub_;
        std::vector<double> elems_, rowlb_, rowub_;
        std::vector<int> rows_, cols_;

        void set(int row, int col, double coef)
        {
            rows_.push_back(row);
            cols_.push_back(col);
            elems_.push_back(coef);
        }
};

/** @brief Base class for global 2D motion estimation methods which take frames as input.
 */
class ImageMotionEstimatorBase
{
    public:

        virtual ~ImageMotionEstimatorBase() = default;

        virtual void setMotionModel(MotionModel val)
        {
            motionModel_ = val;
        }

        virtual MotionModel motionModel() const
        {
            return motionModel_;
        }

        virtual void setFrameMask(cv::InputArray mask)
        {
            if (!mask.empty())
                CV_Error(cv::Error::StsNotImplemented, "Mask support is not implemented.");
        }

        virtual cv::Mat estimate( const cv::Mat &frame0,
                                  const cv::Mat &frame1,
                                  bool *ok = 0) = 0;

    protected:

        explicit ImageMotionEstimatorBase(MotionModel model)
        {
            setMotionModel(model);
        }

    private:
        MotionModel motionModel_;
};


class FromFileMotionReader : public ImageMotionEstimatorBase
{
    public:

        FromFileMotionReader(const cv::String &path);

        cv::Mat estimate( const cv::Mat &frame0, const cv::Mat& frame1, bool *ok = 0) override;

    private:
        std::ifstream file_;
};

class CV_EXPORTS ToFileMotionWriter : public ImageMotionEstimatorBase
{
    public:

        ToFileMotionWriter( const cv::String& path,
                            cv::Ptr<ImageMotionEstimatorBase> estimator);

        virtual void setMotionModel(MotionModel val)override
        {
            motionEstimator_->setMotionModel(val);
        }

        virtual MotionModel motionModel() const override
        {
            return motionEstimator_->motionModel();
        }

        virtual void setFrameMask( cv::InputArray mask )override
        {
            motionEstimator_->setFrameMask(mask);
        }

        cv::Mat estimate(const cv::Mat &frame0, const cv::Mat &frame1, bool *ok = 0)override;

    private:
        std::ofstream file_;
        cv::Ptr<ImageMotionEstimatorBase> motionEstimator_;
};

/**
 * @brief Describes a global 2D motion estimation method which uses keypoints detection and optical flow for matching.
 */
class KeypointBasedMotionEstimator : public ImageMotionEstimatorBase
{
    public:

        explicit KeypointBasedMotionEstimator( std::shared_ptr<MotionEstimatorBase> estimator );

        void setMotionModel(MotionModel val) override
        {
            motionEstimator_->setMotionModel(val);
        }

        MotionModel motionModel() const override
        {
            return motionEstimator_->motionModel();
        }

        void setDetector( cv::Ptr<cv::FeatureDetector> val)
        {
            detector_ = val;
        }

        cv::Ptr<cv::FeatureDetector> detector() const
        {
            return detector_;
        }

        void setOpticalFlowEstimator( std::shared_ptr<ISparseOptFlowEstimator> val)
        {
            optFlowEstimator_ = val;
        }

        std::shared_ptr<ISparseOptFlowEstimator> opticalFlowEstimator() const
        {
            return optFlowEstimator_;
        }

        void setOutlierRejector( std::shared_ptr<IOutlierRejector> val)
        {
            outlierRejector_ = val;
        }

        std::shared_ptr<IOutlierRejector> outlierRejector() const
        {
            return outlierRejector_;
        }

        void setFrameMask( cv::InputArray mask) override
        {
            mask_ = mask.getMat();
        }

        cv::Mat estimate( const cv::Mat &frame0,
                          const cv::Mat&frame1,
                          bool *ok = 0) override;

        cv::Mat estimate( cv::InputArray frame0, cv::InputArray frame1, bool *ok = 0);

    private:
        std::shared_ptr<MotionEstimatorBase> motionEstimator_;
        cv::Ptr<cv::FeatureDetector> detector_;
        std::shared_ptr<ISparseOptFlowEstimator> optFlowEstimator_;
        std::shared_ptr<IOutlierRejector> outlierRejector_;
        cv::Mat mask_;

        std::vector<uchar> status_;
        std::vector<cv::KeyPoint> keypointsPrev_;
        std::vector<cv::Point2f> pointsPrev_, points_;
        std::vector<cv::Point2f> pointsPrevGood_, pointsGood_;
};

/** @brief Computes motion between two frames assuming that all the intermediate motions are known.
 *  @param from Source frame index.
 *  @param to Destination frame index.
 *  @param motions Pair-wise motions. motions[i] denotes motion from the frame i to the frame i+1
 *  @return Motion from the Source frame to the Destination frame.
 */
cv::Mat getMotion(int from, int to, const std::vector<cv::Mat> &motions);


#endif

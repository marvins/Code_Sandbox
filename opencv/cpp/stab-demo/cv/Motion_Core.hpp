#ifndef STAB_DEMO_CV_VIDEOSTAB_MOTION_CORE_HPP
#define STAB_DEMO_CV_VIDEOSTAB_MOTION_CORE_HPP

#include <cmath>


#include <opencv2/core.hpp>


enum MotionModel
{
    MM_TRANSLATION = 0,
    MM_TRANSLATION_AND_SCALE = 1,
    MM_ROTATION = 2,
    MM_RIGID = 3,
    MM_SIMILARITY = 4,
    MM_AFFINE = 5,
    MM_HOMOGRAPHY = 6,
    MM_UNKNOWN = 7
};

/** @brief Describes RANSAC method parameters.
 */
struct RansacParams
{
    int size; //!< subset size
    float thresh; //!< max error to classify as inlier
    float eps; //!< max outliers ratio
    float prob; //!< probability of success

    RansacParams() : size(0), thresh(0), eps(0), prob(0) {}
    /** @brief Constructor
    @param size Subset size.
    @param thresh Maximum re-projection error value to classify as inlier.
    @param eps Maximum ratio of incorrect correspondences.
    @param prob Required success probability.
     */
    RansacParams(int _size, float _thresh, float _eps, float _prob)
     : size(_size),
       thresh(_thresh),
       eps(_eps),
       prob(_prob)
    {}

    /**
    @return Number of iterations that'll be performed by RANSAC method.
    */
    int niters() const
    {
        return static_cast<int>(
                std::ceil(std::log(1 - prob) / std::log(1 - std::pow(1 - eps, size))));
    }

    /**
    @param model Motion model. See cv::videostab::MotionModel.
    @return Default RANSAC method parameters for the given motion model.
    */
    static RansacParams default2dMotion(MotionModel model)
    {
        CV_Assert(model < MM_UNKNOWN);
        if (model == MM_TRANSLATION)
            return RansacParams(1, 0.5f, 0.5f, 0.99f);
        if (model == MM_TRANSLATION_AND_SCALE)
            return RansacParams(2, 0.5f, 0.5f, 0.99f);
        if (model == MM_ROTATION)
            return RansacParams(1, 0.5f, 0.5f, 0.99f);
        if (model == MM_RIGID)
            return RansacParams(2, 0.5f, 0.5f, 0.99f);
        if (model == MM_SIMILARITY)
            return RansacParams(2, 0.5f, 0.5f, 0.99f);
        if (model == MM_AFFINE)
            return RansacParams(3, 0.5f, 0.5f, 0.99f);
        return RansacParams(4, 0.5f, 0.5f, 0.99f);
    }
};


#endif

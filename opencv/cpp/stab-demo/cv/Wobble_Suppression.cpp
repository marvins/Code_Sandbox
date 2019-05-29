#include "Wobble_Suppression.hpp"

// Project Libraries
#include "Ring_Buffer.hpp"


WobbleSuppressorBase::WobbleSuppressorBase()
    : frameCount_(0),
      motions_(0),
      motions2_(0),
      stabilizationMotions_(0)
{
    setMotionEstimator( cv::makePtr<KeypointBasedMotionEstimator>( std::make_shared<MotionEstimatorRansacL2>(MM_HOMOGRAPHY)));
}


void NullWobbleSuppressor::suppress(int /*idx*/, const cv::Mat &frame, cv::Mat &result)
{
    result = frame;
}


void MoreAccurateMotionWobbleSuppressor::suppress(int idx, const cv::Mat &frame, cv::Mat &result)
{
    CV_Assert(motions_ && stabilizationMotions_);

    if (idx % period_ == 0)
    {
        result = frame;
        return;
    }

    int k1 = idx / period_ * period_;
    int k2 = std::min(k1 + period_, frameCount_ - 1);

    cv::Mat S1 = (*stabilizationMotions_)[idx];

    cv::Mat_<float> ML = S1 * getMotion(k1, idx, *motions2_) * getMotion(k1, idx, *motions_).inv() * S1.inv();
    cv::Mat_<float> MR = S1 * getMotion(idx, k2, *motions2_).inv() * getMotion(idx, k2, *motions_) * S1.inv();

    mapx_.create(frame.size());
    mapy_.create(frame.size());

    float xl, yl, zl, wl;
    float xr, yr, zr, wr;

    for (int y = 0; y < frame.rows; ++y)
    {
        for (int x = 0; x < frame.cols; ++x)
        {
            xl = ML(0,0)*x + ML(0,1)*y + ML(0,2);
            yl = ML(1,0)*x + ML(1,1)*y + ML(1,2);
            zl = ML(2,0)*x + ML(2,1)*y + ML(2,2);
            xl /= zl; yl /= zl;
            wl = float(idx - k1);

            xr = MR(0,0)*x + MR(0,1)*y + MR(0,2);
            yr = MR(1,0)*x + MR(1,1)*y + MR(1,2);
            zr = MR(2,0)*x + MR(2,1)*y + MR(2,2);
            xr /= zr; yr /= zr;
            wr = float(k2 - idx);

            mapx_(y,x) = (wr * xl + wl * xr) / (wl + wr);
            mapy_(y,x) = (wr * yl + wl * yr) / (wl + wr);
        }
    }

    if (result.data == frame.data)
        result = cv::Mat(frame.size(), frame.type());

    remap(frame, result, mapx_, mapy_, cv::INTER_LINEAR, cv::BORDER_REPLICATE);
}


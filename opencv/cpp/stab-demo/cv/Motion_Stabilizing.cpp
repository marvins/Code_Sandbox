#include "Motion_Stabilizing.hpp"

// Project Libraries
#include "Global_Motion.hpp"
#include "Ring_Buffer.hpp"

// C++ Libraries
#include <iostream>


void MotionFilterBase::stabilize( int size,
                                  const std::vector<cv::Mat> &motions,
                                  std::pair<int,int> range,
                                  cv::Mat *stabilizationMotions)
{
    for (int i = 0; i < size; ++i)
        stabilizationMotions[i] = stabilize(i, motions, range);
}


void GaussianMotionFilter::setParams(int _radius, float _stdev)
{
    radius_ = _radius;
    stdev_ = _stdev > 0.f ? _stdev : std::sqrt(static_cast<float>(_radius));

    float sum = 0;
    weight_.resize(2*radius_ + 1);
    for (int i = -radius_; i <= radius_; ++i)
        sum += weight_[radius_ + i] = std::exp(-i*i/(stdev_*stdev_));
    for (int i = -radius_; i <= radius_; ++i)
        weight_[radius_ + i] /= sum;
}


cv::Mat GaussianMotionFilter::stabilize( int idx,
                                         const std::vector<cv::Mat>& motions,
                                         std::pair<int,int> range)
{
    std::cout << "GaussianMotionFilter::Stabilize. IDX: " << idx << ", Range (" << std::get<0>(range) << ", " << std::get<1>(range) << ")" << std::endl;
    const cv::Mat &cur = at(idx, motions);
    cv::Mat res = cv::Mat::zeros(cur.size(), cur.type());
    float sum = 0.f;
    int iMin = std::max(idx - radius_, range.first);
    int iMax = std::min(idx + radius_, range.second);
    for (int i = iMin; i <= iMax; ++i)
    {
        res += weight_[radius_ + i - idx] * getMotion(idx, i, motions);
        sum += weight_[radius_ + i - idx];
    }
    return sum > 0.f ? res / sum : cv::Mat::eye(cur.size(), cur.type());
}


LpMotionStabilizer::LpMotionStabilizer(MotionModel model)
{
    setMotionModel(model);
    setFrameSize(cv::Size(0,0));
    setTrimRatio(0.1f);
    setWeight1(1);
    setWeight2(10);
    setWeight3(100);
    setWeight4(100);
}

void LpMotionStabilizer::stabilize(int, const std::vector<cv::Mat>&, std::pair<int,int>, cv::Mat*)
{
    CV_Error(cv::Error::StsError, "The library is built without Clp support");
}



static inline int areaSign( cv::Point2f a,
                            cv::Point2f b,
                            cv::Point2f c)
{
    double area = (b-a).cross(c-a);
    if (area < -1e-5) return -1;
    if (area > 1e-5) return 1;
    return 0;
}


static inline bool segmentsIntersect( cv::Point2f a,
                                      cv::Point2f b,
                                      cv::Point2f c,
                                      cv::Point2f d )
{
    return areaSign(a,b,c) * areaSign(a,b,d) < 0 &&
           areaSign(c,d,a) * areaSign(c,d,b) < 0;
}


// Checks if rect a (with sides parallel to axis) is inside rect b (arbitrary).
// Rects must be passed in the [(0,0), (w,0), (w,h), (0,h)] order.
static inline bool isRectInside( const cv::Point2f a[4], const cv::Point2f b[4])
{
    for (int i = 0; i < 4; ++i)
        if (b[i].x > a[0].x && b[i].x < a[2].x && b[i].y > a[0].y && b[i].y < a[2].y)
            return false;
    for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
        if (segmentsIntersect(a[i], a[(i+1)%4], b[j], b[(j+1)%4]))
            return false;
    return true;
}


static inline bool isGoodMotion(const float M[], float w, float h, float dx, float dy)
{
    cv::Point2f pt[4] = { cv::Point2f(0,0),
                          cv::Point2f(w,0),
                          cv::Point2f(w,h),
                          cv::Point2f(0,h)};
    cv::Point2f Mpt[4];

    for (int i = 0; i < 4; ++i)
    {
        Mpt[i].x = M[0]*pt[i].x + M[1]*pt[i].y + M[2];
        Mpt[i].y = M[3]*pt[i].x + M[4]*pt[i].y + M[5];
        float z = M[6]*pt[i].x + M[7]*pt[i].y + M[8];
        Mpt[i].x /= z;
        Mpt[i].y /= z;
    }

    pt[0] = cv::Point2f(dx, dy);
    pt[1] = cv::Point2f(w - dx, dy);
    pt[2] = cv::Point2f(w - dx, h - dy);
    pt[3] = cv::Point2f(dx, h - dy);

    return isRectInside(pt, Mpt);
}


static inline void relaxMotion(const float M[], float t, float res[])
{
    res[0] = M[0]*(1.f-t) + t;
    res[1] = M[1]*(1.f-t);
    res[2] = M[2]*(1.f-t);
    res[3] = M[3]*(1.f-t);
    res[4] = M[4]*(1.f-t) + t;
    res[5] = M[5]*(1.f-t);
    res[6] = M[6]*(1.f-t);
    res[7] = M[7]*(1.f-t);
    res[8] = M[8]*(1.f-t) + t;
}


cv::Mat ensureInclusionConstraint(const cv::Mat &M, cv::Size size, float trimRatio)
{

    CV_Assert(M.size() == cv::Size(3,3) && M.type() == CV_32F);

    const float w = static_cast<float>(size.width);
    const float h = static_cast<float>(size.height);
    const float dx = floor(w * trimRatio);
    const float dy = floor(h * trimRatio);
    const float srcM[] =
            {M.at<float>(0,0), M.at<float>(0,1), M.at<float>(0,2),
             M.at<float>(1,0), M.at<float>(1,1), M.at<float>(1,2),
             M.at<float>(2,0), M.at<float>(2,1), M.at<float>(2,2)};

    float curM[9];
    float t = 0;
    relaxMotion(srcM, t, curM);
    if (isGoodMotion(curM, w, h, dx, dy))
        return M;

    float l = 0, r = 1;
    while (r - l > 1e-3f)
    {
        t = (l + r) * 0.5f;
        relaxMotion(srcM, t, curM);
        if (isGoodMotion(curM, w, h, dx, dy))
            r = t;
        else
            l = t;
    }

    return (1 - r) * M + r * cv::Mat::eye(3, 3, CV_32F);
}


// TODO can be estimated for O(1) time
float estimateOptimalTrimRatio(const cv::Mat &M, cv::Size size)
{
    CV_Assert(M.size() == cv::Size(3,3) && M.type() == CV_32F);

    const float w = static_cast<float>(size.width);
    const float h = static_cast<float>(size.height);
    cv::Mat_<float> M_(M);

    cv::Point2f pt[4] = {cv::Point2f(0,0), cv::Point2f(w,0), cv::Point2f(w,h), cv::Point2f(0,h)};
    cv::Point2f Mpt[4];
    float z;

    for (int i = 0; i < 4; ++i)
    {
        Mpt[i].x = M_(0,0)*pt[i].x + M_(0,1)*pt[i].y + M_(0,2);
        Mpt[i].y = M_(1,0)*pt[i].x + M_(1,1)*pt[i].y + M_(1,2);
        z = M_(2,0)*pt[i].x + M_(2,1)*pt[i].y + M_(2,2);
        Mpt[i].x /= z;
        Mpt[i].y /= z;
    }

    float l = 0, r = 0.5f;
    while (r - l > 1e-3f)
    {
        float t = (l + r) * 0.5f;
        float dx = floor(w * t);
        float dy = floor(h * t);
        pt[0] = cv::Point2f(dx, dy);
        pt[1] = cv::Point2f(w - dx, dy);
        pt[2] = cv::Point2f(w - dx, h - dy);
        pt[3] = cv::Point2f(dx, h - dy);
        if (isRectInside(pt, Mpt))
            r = t;
        else
            l = t;
    }

    return r;
}


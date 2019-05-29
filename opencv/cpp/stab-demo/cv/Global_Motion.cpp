#include "Global_Motion.hpp"

// Project Libraries
#include "Ring_Buffer.hpp"
#include "Outlier_Rejection.hpp"

// OpenCV Libraries
#include <opencv2/calib3d.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>


/****************************************************/
/*          does isotropic normalization            */
/****************************************************/
static cv::Mat normalizePoints( int npoints, cv::Point2f *points )
{
    float cx = 0.f, cy = 0.f;
    for (int i = 0; i < npoints; ++i)
    {
        cx += points[i].x;
        cy += points[i].y;
    }
    cx /= npoints;
    cy /= npoints;

    float d = 0.f;
    for (int i = 0; i < npoints; ++i)
    {
        points[i].x -= cx;
        points[i].y -= cy;
        d += std::sqrt((points[i].x * points[i].x) + (points[i].y * points[i].y));
    }
    d /= npoints;

    float s = std::sqrt(2.f) / d;
    for (int i = 0; i < npoints; ++i)
    {
        points[i].x *= s;
        points[i].y *= s;
    }

    cv::Mat_<float> T = cv::Mat::eye(3, 3, CV_32F);
    T(0,0) = T(1,1) = s;
    T(0,2) = -cx*s;
    T(1,2) = -cy*s;
    return std::move(T);
}


/****************************************************************/
/*          Estimate Global Motion using Least Squares          */
/****************************************************************/
static cv::Mat estimateGlobMotionLeastSquaresTranslation( int npoints,
                                                          cv::Point2f *points0,
                                                          cv::Point2f *points1,
                                                          float *rmse)
{
    cv::Mat_<float> M = cv::Mat::eye(3, 3, CV_32F);
    for (int i = 0; i < npoints; ++i)
    {
        M(0,2) += points1[i].x - points0[i].x;
        M(1,2) += points1[i].y - points0[i].y;
    }
    M(0,2) /= npoints;
    M(1,2) /= npoints;

    if (rmse)
    {
        *rmse = 0;
        for (int i = 0; i < npoints; ++i)
            *rmse += std::pow((double)points1[i].x - points0[i].x - M(0,2),2.0) +
                     std::pow((double)points1[i].y - points0[i].y - M(1,2),2.0);
        *rmse = std::sqrt(*rmse / npoints);
    }

    return std::move(M);
}


/****************************************************************/
/*          Estimate Global Motion using Least Squares          */
/****************************************************************/
static cv::Mat estimateGlobMotionLeastSquaresTranslationAndScale( int npoints,
                                                                  cv::Point2f *points0,
                                                                  cv::Point2f *points1,
                                                                  float *rmse)
{
    cv::Mat_<float> T0 = normalizePoints(npoints, points0);
    cv::Mat_<float> T1 = normalizePoints(npoints, points1);

    cv::Mat_<float> A(2*npoints, 3), b(2*npoints, 1);
    float *a0, *a1;
    cv::Point2f p0, p1;

    for (int i = 0; i < npoints; ++i)
    {
        a0 = A[2*i];
        a1 = A[2*i+1];
        p0 = points0[i];
        p1 = points1[i];
        a0[0] = p0.x; a0[1] = 1; a0[2] = 0;
        a1[0] = p0.y; a1[1] = 0; a1[2] = 1;
        b(2*i,0) = p1.x;
        b(2*i+1,0) = p1.y;
    }

    cv::Mat_<float> sol;
    cv::solve(A, b, sol, cv::DECOMP_NORMAL | cv::DECOMP_LU);

    if (rmse)
        *rmse = static_cast<float>(norm(A*sol, b, cv::NORM_L2) / std::sqrt(static_cast<double>(npoints)));

    cv::Mat_<float> M = cv::Mat::eye(3, 3, CV_32F);
    M(0,0) = M(1,1) = sol(0,0);
    M(0,2) = sol(1,0);
    M(1,2) = sol(2,0);

    return T1.inv() * M * T0;
}

/****************************************************************/
/*          Estimate Global Motion using Least Squares          */
/****************************************************************/
static cv::Mat estimateGlobMotionLeastSquaresRotation( int npoints,
                                                       cv::Point2f *points0,
                                                       cv::Point2f *points1,
                                                       float *rmse)
{
    cv::Point2f p0, p1;
    float A(0), B(0);
    for(int i=0; i<npoints; ++i)
    {
        p0 = points0[i];
        p1 = points1[i];

        A += p0.x*p1.x + p0.y*p1.y;
        B += p0.x*p1.y - p1.x*p0.y;
    }

    // A*sin(alpha) + B*cos(alpha) = 0
    float C = std::sqrt(A*A + B*B);
    cv::Mat_<float> M = cv::Mat::eye(3, 3, CV_32F);
    if ( C != 0 )
    {
        float sinAlpha = - B / C;
        float cosAlpha = A / C;

        M(0,0) = cosAlpha;
        M(1,1) = M(0,0);
        M(0,1) = sinAlpha;
        M(1,0) = - M(0,1);
    }

    if (rmse)
    {
        *rmse = 0;
        for (int i = 0; i < npoints; ++i)
        {
            p0 = points0[i];
            p1 = points1[i];
            *rmse += std::pow(p1.x - M(0,0)*p0.x - M(0,1)*p0.y, 2.0) +
                     std::pow(p1.y - M(1,0)*p0.x - M(1,1)*p0.y, 2.0);
        }
        *rmse = std::sqrt(*rmse / npoints);
    }

    return std::move(M);
}

static cv::Mat  estimateGlobMotionLeastSquaresRigid( int npoints,
                                                     cv::Point2f *points0,
                                                     cv::Point2f *points1,
                                                     float *rmse)
{
    cv::Point2f mean0(0.f, 0.f);
    cv::Point2f mean1(0.f, 0.f);

    for (int i = 0; i < npoints; ++i)
    {
        mean0 += points0[i];
        mean1 += points1[i];
    }

    mean0 *= 1.f / npoints;
    mean1 *= 1.f / npoints;

    cv::Mat_<float> A = cv::Mat::zeros(2, 2, CV_32F);
    cv::Point2f pt0, pt1;

    for (int i = 0; i < npoints; ++i)
    {
        pt0 = points0[i] - mean0;
        pt1 = points1[i] - mean1;
        A(0,0) += pt1.x * pt0.x;
        A(0,1) += pt1.x * pt0.y;
        A(1,0) += pt1.y * pt0.x;
        A(1,1) += pt1.y * pt0.y;
    }

    cv::Mat_<float> M = cv::Mat::eye(3, 3, CV_32F);

    cv::SVD svd(A);
    cv::Mat_<float> R = svd.u * svd.vt;
    cv::Mat tmp(M(cv::Rect(0,0,2,2)));
    R.copyTo(tmp);

    M(0,2) = mean1.x - R(0,0)*mean0.x - R(0,1)*mean0.y;
    M(1,2) = mean1.y - R(1,0)*mean0.x - R(1,1)*mean0.y;

    if (rmse)
    {
        *rmse = 0;
        for (int i = 0; i < npoints; ++i)
        {
            pt0 = points0[i];
            pt1 = points1[i];
            *rmse += std::pow(pt1.x - M(0,0)*pt0.x - M(0,1)*pt0.y - M(0,2), 2.0) +
                     std::pow(pt1.y - M(1,0)*pt0.x - M(1,1)*pt0.y - M(1,2), 2.0);
        }
        *rmse = std::sqrt(*rmse / npoints);
    }

    return std::move(M);
}


static cv::Mat estimateGlobMotionLeastSquaresSimilarity( int npoints,
                                                         cv::Point2f *points0,
                                                         cv::Point2f *points1,
                                                         float *rmse)
{
    cv::Mat_<float> T0 = normalizePoints(npoints, points0);
    cv::Mat_<float> T1 = normalizePoints(npoints, points1);

    cv::Mat_<float> A(2*npoints, 4), b(2*npoints, 1);
    float *a0, *a1;
    cv::Point2f p0, p1;

    for (int i = 0; i < npoints; ++i)
    {
        a0 = A[2*i];
        a1 = A[2*i+1];
        p0 = points0[i];
        p1 = points1[i];
        a0[0] = p0.x; a0[1] = p0.y; a0[2] = 1; a0[3] = 0;
        a1[0] = p0.y; a1[1] = -p0.x; a1[2] = 0; a1[3] = 1;
        b(2*i,0) = p1.x;
        b(2*i+1,0) = p1.y;
    }

    cv::Mat_<float> sol;
    cv::solve(A, b, sol, cv::DECOMP_NORMAL | cv::DECOMP_LU);

    if (rmse)
        *rmse = static_cast<float>(norm(A*sol, b, cv::NORM_L2) / std::sqrt(static_cast<double>(npoints)));

    cv::Mat_<float> M = cv::Mat::eye(3, 3, CV_32F);
    M(0,0) = M(1,1) = sol(0,0);
    M(0,1) = sol(1,0);
    M(1,0) = -sol(1,0);
    M(0,2) = sol(2,0);
    M(1,2) = sol(3,0);

    return T1.inv() * M * T0;
}


static cv::Mat estimateGlobMotionLeastSquaresAffine( int npoints,
                                                     cv::Point2f *points0,
                                                     cv::Point2f *points1,
                                                     float *rmse)
{
    cv::Mat_<float> T0 = normalizePoints(npoints, points0);
    cv::Mat_<float> T1 = normalizePoints(npoints, points1);

    cv::Mat_<float> A(2*npoints, 6), b(2*npoints, 1);
    float *a0, *a1;
    cv::Point2f p0, p1;

    for (int i = 0; i < npoints; ++i)
    {
        a0 = A[2*i];
        a1 = A[2*i+1];
        p0 = points0[i];
        p1 = points1[i];
        a0[0] = p0.x; a0[1] = p0.y; a0[2] = 1; a0[3] = a0[4] = a0[5] = 0;
        a1[0] = a1[1] = a1[2] = 0; a1[3] = p0.x; a1[4] = p0.y; a1[5] = 1;
        b(2*i,0) = p1.x;
        b(2*i+1,0) = p1.y;
    }

    cv::Mat_<float> sol;
    cv::solve(A, b, sol, cv::DECOMP_NORMAL | cv::DECOMP_LU);

    if (rmse)
        *rmse = static_cast<float>(norm(A*sol, b, cv::NORM_L2) / std::sqrt(static_cast<double>(npoints)));

    cv::Mat_<float> M = cv::Mat::eye(3, 3, CV_32F);
    for (int i = 0, k = 0; i < 2; ++i)
        for (int j = 0; j < 3; ++j, ++k)
            M(i,j) = sol(k,0);

    return T1.inv() * M * T0;
}


cv::Mat estimateGlobalMotionLeastSquares( cv::InputOutputArray points0,
                                          cv::InputOutputArray points1,
                                          int model,
                                          float *rmse)
{
    CV_Assert(model <= MM_AFFINE);
    CV_Assert(points0.type() == points1.type());
    const int npoints = points0.getMat().checkVector(2);
    CV_Assert(points1.getMat().checkVector(2) == npoints);

    typedef cv::Mat (*Impl)(int, cv::Point2f*, cv::Point2f*, float*);
    static Impl impls[] = { estimateGlobMotionLeastSquaresTranslation,
                            estimateGlobMotionLeastSquaresTranslationAndScale,
                            estimateGlobMotionLeastSquaresRotation,
                            estimateGlobMotionLeastSquaresRigid,
                            estimateGlobMotionLeastSquaresSimilarity,
                            estimateGlobMotionLeastSquaresAffine };

    cv::Point2f *points0_ = points0.getMat().ptr<cv::Point2f>();
    cv::Point2f *points1_ = points1.getMat().ptr<cv::Point2f>();

    return impls[model](npoints, points0_, points1_, rmse);
}


cv::Mat estimateGlobalMotionRansac( cv::InputArray points0,
                                    cv::InputArray points1,
                                    int model,
                                    const RansacParams &params,
                                    float *rmse, int *ninliers)
{
    CV_Assert(model <= MM_AFFINE);
    CV_Assert(points0.type() == points1.type());
    const int npoints = points0.getMat().checkVector(2);
    CV_Assert(points1.getMat().checkVector(2) == npoints);

    if (npoints < params.size)
        return cv::Mat::eye(3, 3, CV_32F);

    const cv::Point2f *points0_ = points0.getMat().ptr<cv::Point2f>();
    const cv::Point2f *points1_ = points1.getMat().ptr<cv::Point2f>();
    const int niters = params.niters();

    // current hypothesis
    std::vector<int> indices(params.size);
    std::vector<cv::Point2f> subset0(params.size);
    std::vector<cv::Point2f> subset1(params.size);

    // best hypothesis
    std::vector<int> bestIndices(params.size);

    cv::Mat_<float> bestM;
    int ninliersMax = -1;

    cv::RNG rng(0);
    cv::Point2f p0, p1;
    float x, y;

    for (int iter = 0; iter < niters; ++iter)
    {
        for (int i = 0; i < params.size; ++i)
        {
            bool ok = false;
            while (!ok)
            {
                ok = true;
                indices[i] = static_cast<unsigned>(rng) % npoints;
                for (int j = 0; j < i; ++j)
                    if (indices[i] == indices[j])
                        { ok = false; break; }
            }
        }
        for (int i = 0; i < params.size; ++i)
        {
            subset0[i] = points0_[indices[i]];
            subset1[i] = points1_[indices[i]];
        }

        cv::Mat_<float> M = estimateGlobalMotionLeastSquares(subset0, subset1, model, 0);

        int numinliers = 0;
        for (int i = 0; i < npoints; ++i)
        {
            p0 = points0_[i];
            p1 = points1_[i];
            x = M(0,0)*p0.x + M(0,1)*p0.y + M(0,2);
            y = M(1,0)*p0.x + M(1,1)*p0.y + M(1,2);
            if (std::pow(x - p1.x, 2.0) + std::pow(y - p1.y, 2.0) < params.thresh * params.thresh)
                numinliers++;
        }
        if (numinliers >= ninliersMax)
        {
            bestM = M;
            ninliersMax = numinliers;
            bestIndices.swap(indices);
        }
    }

    if (ninliersMax < params.size)
    {
        // compute RMSE
        for (int i = 0; i < params.size; ++i)
        {
            subset0[i] = points0_[bestIndices[i]];
            subset1[i] = points1_[bestIndices[i]];
        }
        bestM = estimateGlobalMotionLeastSquares(subset0, subset1, model, rmse);
    }
    else
    {
        subset0.resize(ninliersMax);
        subset1.resize(ninliersMax);
        for (int i = 0, j = 0; i < npoints && j < ninliersMax ; ++i)
        {
            p0 = points0_[i];
            p1 = points1_[i];
            x = bestM(0,0)*p0.x + bestM(0,1)*p0.y + bestM(0,2);
            y = bestM(1,0)*p0.x + bestM(1,1)*p0.y + bestM(1,2);
            if (std::pow(x - p1.x, 2.0) + std::pow(y - p1.y, 2.0) < params.thresh * params.thresh)
            {
                subset0[j] = p0;
                subset1[j] = p1;
                j++;
            }
        }
        bestM = estimateGlobalMotionLeastSquares(subset0, subset1, model, rmse);
    }

    if (ninliers)
        *ninliers = ninliersMax;

    return std::move(bestM);
}


MotionEstimatorRansacL2::MotionEstimatorRansacL2(MotionModel model)
    : MotionEstimatorBase(model)
{
    setRansacParams(RansacParams::default2dMotion(model));
    setMinInlierRatio(0.1f);
}


cv::Mat MotionEstimatorRansacL2::estimate( cv::InputArray points0, cv::InputArray points1, bool *ok)
{
    CV_Assert(points0.type() == points1.type());
    const int npoints = points0.getMat().checkVector(2);
    CV_Assert(points1.getMat().checkVector(2) == npoints);

    // find motion

    int ninliers = 0;
    cv::Mat_<float> M;

    if (motionModel() != MM_HOMOGRAPHY)
        M = estimateGlobalMotionRansac(points0, points1, motionModel(), ransacParams_, 0, &ninliers);
    else
    {
        std::vector<uchar> mask;
        M = cv::findHomography(points0, points1, mask, cv::LMEDS);
        for (int i  = 0; i < npoints; ++i)
            if (mask[i]) ninliers++;
    }

    // check if we're confident enough in estimated motion

    if (ok) *ok = true;
    if (static_cast<float>(ninliers) / npoints < minInlierRatio_)
    {
        M = cv::Mat::eye(3, 3, CV_32F);
        if (ok) *ok = false;
    }

    return std::move(M);
}


MotionEstimatorL1::MotionEstimatorL1(MotionModel model)
    : MotionEstimatorBase(model)
{
}


/****************************************/
/*          Estimate L1 Motion          */
/****************************************/
cv::Mat MotionEstimatorL1::estimate( cv::InputArray points0, cv::InputArray points1, bool *ok)
{
    CV_Assert(points0.type() == points1.type());
    const int npoints = points0.getMat().checkVector(2);
    CV_Assert(points1.getMat().checkVector(2) == npoints);

    CV_UNUSED(ok);
    CV_Error(cv::Error::StsError, "The library is built without Clp support");
    return cv::Mat();
}


/********************************/
/*          Constructor         */
/********************************/
FromFileMotionReader::FromFileMotionReader(const cv::String &path)
    : ImageMotionEstimatorBase(MM_UNKNOWN)
{
    file_.open(path.c_str());
    CV_Assert(file_.is_open());
}


/********************************/
/*          Estimate            */
/********************************/
cv::Mat FromFileMotionReader::estimate( const cv::Mat& /*frame0*/,
                                        const cv::Mat&/*frame1*/,
                                        bool *ok)
{
    cv::Mat_<float> M(3, 3);
    bool ok_;
    file_ >> M(0,0) >> M(0,1) >> M(0,2)
          >> M(1,0) >> M(1,1) >> M(1,2)
          >> M(2,0) >> M(2,1) >> M(2,2) >> ok_;
    if (ok) *ok = ok_;
    return std::move(M);
}


/********************************/
/*          Constructor         */
/********************************/
ToFileMotionWriter::ToFileMotionWriter( const cv::String &path,
                                        cv::Ptr<ImageMotionEstimatorBase> estimator)
    : ImageMotionEstimatorBase(estimator->motionModel()),
      motionEstimator_(estimator)
{
    file_.open(path.c_str());
    CV_Assert(file_.is_open());
}


/********************************/
/*          Estimate            */
/********************************/
cv::Mat ToFileMotionWriter::estimate( const cv::Mat& frame0,
                                      const cv::Mat& frame1,
                                      bool *ok)
{
    bool ok_;
    cv::Mat_<float> M = motionEstimator_->estimate(frame0, frame1, &ok_);
    file_ << M(0,0) << " " << M(0,1) << " " << M(0,2) << " "
          << M(1,0) << " " << M(1,1) << " " << M(1,2) << " "
          << M(2,0) << " " << M(2,1) << " " << M(2,2) << " " << ok_ << std::endl;
    if (ok) *ok = ok_;
    return std::move(M);
}


/********************************/
/*          Constructor         */
/********************************/
KeypointBasedMotionEstimator::KeypointBasedMotionEstimator( std::shared_ptr<MotionEstimatorBase> estimator)
    : ImageMotionEstimatorBase(estimator->motionModel()),
      motionEstimator_(estimator)
{
    setDetector(cv::GFTTDetector::create());
    setOpticalFlowEstimator(std::make_shared<SparsePyrLkOptFlowEstimator>());
    setOutlierRejector(std::make_shared<NullOutlierRejector>());
}


/************************************************/
/*          Estimate Keypoint Motion            */
/************************************************/
cv::Mat KeypointBasedMotionEstimator::estimate( const cv::Mat& frame0,
                                            const cv::Mat& frame1,
                                            bool *ok)
{
    cv::InputArray image0 = frame0;
    cv::InputArray image1 = frame1;

    return estimate(image0, image1, ok);
}


/************************************************/
/*          Estimate Keypoint Motion            */
/************************************************/
cv::Mat KeypointBasedMotionEstimator::estimate( cv::InputArray frame0,
                                                cv::InputArray frame1,
                                                bool *ok)
{
    // find keypoints
    detector_->detect(frame0, keypointsPrev_, mask_);
    if (keypointsPrev_.empty())
        return cv::Mat::eye(3, 3, CV_32F);

    // extract points from keypoints
    pointsPrev_.resize(keypointsPrev_.size());
    for (size_t i = 0; i < keypointsPrev_.size(); ++i)
        pointsPrev_[i] = keypointsPrev_[i].pt;

    // find correspondences
    optFlowEstimator_->run(frame0, frame1, pointsPrev_, points_, status_, cv::noArray());

    // leave good correspondences only

    pointsPrevGood_.clear(); pointsPrevGood_.reserve(points_.size());
    pointsGood_.clear(); pointsGood_.reserve(points_.size());

    for (size_t i = 0; i < points_.size(); ++i)
    {
        if (status_[i])
        {
            pointsPrevGood_.push_back(pointsPrev_[i]);
            pointsGood_.push_back(points_[i]);
        }
    }

    // perform outlier rejection

    IOutlierRejector *outlRejector = outlierRejector_.get();
    if (!dynamic_cast<NullOutlierRejector*>(outlRejector))
    {
        pointsPrev_.swap(pointsPrevGood_);
        points_.swap(pointsGood_);

        outlierRejector_->process(frame0.size(), pointsPrev_, points_, status_);

        pointsPrevGood_.clear();
        pointsPrevGood_.reserve(points_.size());

        pointsGood_.clear();
        pointsGood_.reserve(points_.size());

        for (size_t i = 0; i < points_.size(); ++i)
        {
            if (status_[i])
            {
                pointsPrevGood_.push_back(pointsPrev_[i]);
                pointsGood_.push_back(points_[i]);
            }
        }
    }

    // estimate motion
    return motionEstimator_->estimate(pointsPrevGood_, pointsGood_, ok);
}


/************************************************/
/*          Get the Requested Motion            */
/************************************************/
cv::Mat getMotion(int from, int to, const std::vector<cv::Mat> &motions)
{
    cv::Mat M = cv::Mat::eye(3, 3, CV_32F);
    if (to > from)
    {
        for (int i = from; i < to; ++i)
            M = at(i, motions) * M;
    }
    else if (from > to)
    {
        for (int i = to; i < from; ++i)
            M = at(i, motions) * M;
        M = M.inv();
    }
    return M;
}

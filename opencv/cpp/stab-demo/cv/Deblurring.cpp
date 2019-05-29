/**
 * @file    Deblurring.cpp
 * @author  Marvin Smith
 * @date    5/29/2019
 */
#include "Deblurring.hpp"

// Project Libraries
#include "Global_Motion.hpp"
#include "Ring_Buffer.hpp"


/********************************************************/
/*          Calculate the Blurriness of a Frame         */
/********************************************************/
float calcBlurriness( const cv::Mat& frame )
{
    cv::Mat Gx, Gy;
    cv::Sobel(frame, Gx, CV_32F, 1, 0);
    cv::Sobel(frame, Gy, CV_32F, 0, 1);
    double normGx = norm(Gx);
    double normGy = norm(Gy);
    double sumSq = normGx*normGx + normGy*normGy;
    return static_cast<float>(1. / (sumSq / frame.size().area() + 1e-6));
}


WeightingDeblurer::WeightingDeblurer()
{
    setSensitivity(0.1f);
}


void WeightingDeblurer::deblur(int idx, cv::Mat &frame)
{
    CV_Assert(frame.type() == CV_8UC3);

    bSum_.create(frame.size());
    gSum_.create(frame.size());
    rSum_.create(frame.size());
    wSum_.create(frame.size());

    for (int y = 0; y < frame.rows; ++y)
    {
        for (int x = 0; x < frame.cols; ++x)
        {
            cv::Point3_<uchar> p = frame.at<cv::Point3_<uchar> >(y,x);
            bSum_(y,x) = p.x;
            gSum_(y,x) = p.y;
            rSum_(y,x) = p.z;
            wSum_(y,x) = 1.f;
        }
    }

    for (int k = idx - radius_; k <= idx + radius_; ++k)
    {
        const cv::Mat &neighbor = at(k, *frames_);
        float bRatio = at(idx, *blurrinessRates_) / at(k, *blurrinessRates_);
        cv::Mat_<float> M = getMotion(idx, k, *motions_);

        if (bRatio > 1.f)
        {
            for (int y = 0; y < frame.rows; ++y)
            {
                for (int x = 0; x < frame.cols; ++x)
                {
                    int x1 = cvRound(M(0,0)*x + M(0,1)*y + M(0,2));
                    int y1 = cvRound(M(1,0)*x + M(1,1)*y + M(1,2));

                    if (x1 >= 0 && x1 < neighbor.cols && y1 >= 0 && y1 < neighbor.rows)
                    {
                        const cv::Point3_<uchar> &p = frame.at<cv::Point3_<uchar> >(y,x);
                        const cv::Point3_<uchar> &p1 = neighbor.at<cv::Point3_<uchar> >(y1,x1);
                        float w = bRatio * sensitivity_ /
                                (sensitivity_ + std::abs(intensity(p1) - intensity(p)));
                        bSum_(y,x) += w * p1.x;
                        gSum_(y,x) += w * p1.y;
                        rSum_(y,x) += w * p1.z;
                        wSum_(y,x) += w;
                    }
                }
            }
        }
    }

    for (int y = 0; y < frame.rows; ++y)
    {
        for (int x = 0; x < frame.cols; ++x)
        {
            float wSumInv = 1.f / wSum_(y,x);
            frame.at<cv::Point3_<uchar> >(y,x) = cv::Point3_<uchar>(
                    static_cast<uchar>(bSum_(y,x)*wSumInv),
                    static_cast<uchar>(gSum_(y,x)*wSumInv),
                    static_cast<uchar>(rSum_(y,x)*wSumInv));
        }
    }
}

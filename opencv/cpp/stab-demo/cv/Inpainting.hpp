/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009-2011, Willow Garage Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#ifndef OPENCV_VIDEOSTAB_INPAINTINT_HPP
#define OPENCV_VIDEOSTAB_INPAINTINT_HPP

// C++ Libraries
#include <vector>

// OpenCV Libraries
#include <opencv2/core.hpp>
#include <opencv2/photo.hpp>

// Project Libraries
#include "Optical_Flow.hpp"
#include "Fast_Marching.hpp"
#include "Global_Motion.hpp"


class InpainterBase
{
    public:

        InpainterBase()
            : radius_(0),
              motionModel_(MM_UNKNOWN),
              frames_(0),
              motions_(0),
              stabilizedFrames_(0),
              stabilizationMotions_(0)
        {}

        virtual ~InpainterBase() = default;

        virtual void setRadius(int val)
        {
            radius_ = val;
        }

        virtual int radius() const
        {
            return radius_;
        }

        virtual void setMotionModel(MotionModel val)
        {
            motionModel_ = val;
        }

        virtual MotionModel motionModel() const
        {
            return motionModel_;
        }

        virtual void inpaint( int idx, cv::Mat &frame, cv::Mat &mask) = 0;


        // data from stabilizer

        virtual void setFrames(const std::vector<cv::Mat> &val)
        {
            frames_ = &val;
        }

        virtual const std::vector<cv::Mat> &frames() const
        {
            return *frames_;
        }

        virtual void setMotions(const std::vector<cv::Mat> &val)
        {
            motions_ = &val;
        }

        virtual const std::vector<cv::Mat> &motions() const
        {
            return *motions_;
        }

        virtual void setStabilizedFrames(const std::vector<cv::Mat> &val)
        {
            stabilizedFrames_ = &val;
        }

        virtual const std::vector<cv::Mat> &stabilizedFrames() const
        {
            return *stabilizedFrames_;
        }

        virtual void setStabilizationMotions(const std::vector<cv::Mat> &val)
        {
            stabilizationMotions_ = &val;
        }

        virtual const std::vector<cv::Mat>& stabilizationMotions() const
        {
            return *stabilizationMotions_;
        }

    protected:

        int radius_;
        MotionModel motionModel_;
        const std::vector<cv::Mat> *frames_;
        const std::vector<cv::Mat> *motions_;
        const std::vector<cv::Mat> *stabilizedFrames_;
        const std::vector<cv::Mat> *stabilizationMotions_;
};

class NullInpainter : public InpainterBase
{
    public:

        virtual void inpaint(int /*idx*/, cv::Mat &/*frame*/, cv::Mat &/*mask*/) override
        {
        }
};

class InpaintingPipeline : public InpainterBase
{
    public:

        void pushBack( std::shared_ptr<InpainterBase> inpainter)
        {
            inpainters_.push_back(inpainter);
        }

        bool empty() const
        {
            return inpainters_.empty();
        }

        void setRadius(int val) override;

        void setMotionModel(MotionModel val) override;


        void setFrames(const std::vector<cv::Mat> &val) override;

        void setMotions(const std::vector<cv::Mat> &val) override;

        void setStabilizedFrames(const std::vector<cv::Mat> &val) override;

        void setStabilizationMotions(const std::vector<cv::Mat> &val) override;

        void inpaint(int idx, cv::Mat &frame, cv::Mat &mask) override;

    private:

        std::vector<std::shared_ptr<InpainterBase>> inpainters_;
};

class ConsistentMosaicInpainter : public InpainterBase
{
    public:

        ConsistentMosaicInpainter();

        void setStdevThresh(float val)
        {
            stdevThresh_ = val;
        }

        float stdevThresh() const
        {
            return stdevThresh_;
        }

        void inpaint(int idx, cv::Mat &frame, cv::Mat &mask) override;

    private:
        float stdevThresh_;
};

class MotionInpainter : public InpainterBase
{
    public:
        MotionInpainter();

        void setOptFlowEstimator(std::shared_ptr<IDenseOptFlowEstimator> val)
        {
            optFlowEstimator_ = val;
        }

        std::shared_ptr<IDenseOptFlowEstimator> optFlowEstimator() const
        {
            return optFlowEstimator_;
        }

        void setFlowErrorThreshold(float val)
        {
            flowErrorThreshold_ = val;
        }

        float flowErrorThreshold() const
        {
            return flowErrorThreshold_;
        }

        void setDistThreshold(float val)
        {
            distThresh_ = val;
        }

        float distThresh() const
        {
            return distThresh_;
        }

        void setBorderMode(int val)
        {
            borderMode_ = val;
        }

        int borderMode() const
        {
            return borderMode_;
        }

        void inpaint(int idx, cv::Mat &frame, cv::Mat &mask) override;

    private:
        FastMarchingMethod fmm_;
        std::shared_ptr<IDenseOptFlowEstimator> optFlowEstimator_;
        float flowErrorThreshold_;
        float distThresh_;
        int borderMode_;

        cv::Mat frame1_, transformedFrame1_;
        cv::Mat_<uchar> grayFrame_, transformedGrayFrame1_;
        cv::Mat_<uchar> mask1_, transformedMask1_;
        cv::Mat_<float> flowX_, flowY_, flowErrors_;
        cv::Mat_<uchar> flowMask_;
};

class ColorAverageInpainter : public InpainterBase
{
    public:

        void inpaint(int idx, cv::Mat &frame, cv::Mat &mask) override;

    private:
        FastMarchingMethod fmm_;
};

class ColorInpainter : public InpainterBase
{
    public:

        ColorInpainter(int method = cv::INPAINT_TELEA, double radius = 2.)
          : method_(method),
            radius_(radius)
        {
        }

        void inpaint(int idx, cv::Mat &frame, cv::Mat &mask) override;

    private:
        int method_;
        double radius_;
        cv::Mat invMask_;
};

void calcFlowMask( const cv::Mat&  flowX,
                   const cv::Mat&  flowY,
                   const cv::Mat&  errors,
                   float           maxError,
                   const cv::Mat&  mask0,
                   const cv::Mat&  mask1,
                   cv::Mat&        flowMask);


void completeFrameAccordingToFlow( const cv::Mat&  flowMask,
                                   const cv::Mat&  flowX,
                                   const cv::Mat&  flowY,
                                   const cv::Mat&  frame1,
                                   const cv::Mat&  mask1,
                                   float           distThresh,
                                   cv::Mat&        frame0,
                                   cv::Mat&        mask0 );


#endif

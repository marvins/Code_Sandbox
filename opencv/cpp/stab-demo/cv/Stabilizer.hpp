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

#ifndef STAB_DEMO_CV_VIDEOSTAB_STABILIZER_HPP
#define STAB_DEMO_CV_VIDEOSTAB_STABILIZER_HPP

// C++ Libraries
#include <ctime>
#include <memory>
#include <vector>

// OpenCV Libraries
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

// Stabilization Implementations
#include "Global_Motion.hpp"
#include "Motion_Stabilizing.hpp"
#include "Frame_Source.hpp"
#include "Log.hpp"
#include "Inpainting.hpp"
#include "Deblurring.hpp"
#include "Wobble_Suppression.hpp"


class StabilizerBase : public IFrameSource
{
    public:

        StabilizerBase();

        virtual ~StabilizerBase() = default;

        void setLog(std::shared_ptr<ILog> ilog)
        {
            log_ = ilog;
        }

        std::shared_ptr<ILog> log() const
        {
            return log_;
        }

        void setRadius(int val)
        {
            radius_ = val;
        }

        int radius() const
        {
            return radius_;
        }

        void setFrameSource( std::shared_ptr<IFrameSource> val)
        {
            frameSource_ = val;
        }

        std::shared_ptr<IFrameSource> frameSource() const
        {
            return frameSource_;
        }

        void setMaskSource(const std::shared_ptr<IFrameSource> &val)
        {
            maskSource_ = val;
        }

        std::shared_ptr<IFrameSource> maskSource() const
        {
            return maskSource_;
        }

        void setMotionEstimator( cv::Ptr<ImageMotionEstimatorBase> val)
        {
            motionEstimator_ = val;
        }

        cv::Ptr<ImageMotionEstimatorBase> motionEstimator() const
        {
            return motionEstimator_;
        }

        void setDeblurer(std::shared_ptr<DeblurerBase> val)
        {
            deblurer_ = val;
        }

        std::shared_ptr<DeblurerBase> deblurrer() const
        {
            return deblurer_;
        }

        void setTrimRatio(float val)
        {
            trimRatio_ = val;
        }

        float trimRatio() const
        {
            return trimRatio_;
        }

        void setCorrectionForInclusion(bool val)
        {
            doCorrectionForInclusion_ = val;
        }

        bool doCorrectionForInclusion() const
        {
            return doCorrectionForInclusion_;
        }

        void setBorderMode(int val)
        {
            borderMode_ = val;
        }

        int borderMode() const
        {
            return borderMode_;
        }

        void setInpainter( std::shared_ptr<InpainterBase> val)
        {
            inpainter_ = val;
        }

        std::shared_ptr <InpainterBase> inpainter() const
        {
            return inpainter_;
        }

        void setMotionStabilizer( std::shared_ptr<IMotionStabilizer> val)
        {
            motionStabilizer_ = val;
        }

        std::shared_ptr<IMotionStabilizer> motionStabilizer() const
        {
            return motionStabilizer_;
        }

        void setWobbleSuppressor( std::shared_ptr<WobbleSuppressorBase> val)
        {
            wobbleSuppressor_ = val;
        }

        std::shared_ptr<WobbleSuppressorBase> wobbleSuppressor() const
        {
            return wobbleSuppressor_;
        }

        void setEstimateTrimRatio(bool val)
        {
            mustEstTrimRatio_ = val;
        }

        bool mustEstimateTrimaRatio() const
        {
            return mustEstTrimRatio_;
        }

        cv::Mat nextFrame() final;

        void reset() final;

    protected:

        cv::Mat nextStabilizedFrame();

        bool doOneIteration();

        virtual void setUp(const cv::Mat &firstFrame);

        virtual cv::Mat estimateMotion();

        virtual cv::Mat estimateStabilizationMotion();

        void stabilizeFrame();

        void runPrePassIfNecessary();

        virtual cv::Mat postProcessFrame(const cv::Mat &frame);

        void logProcessingTime();

        std::shared_ptr<ILog> log_;
        std::shared_ptr<IFrameSource> frameSource_;
        std::shared_ptr<IFrameSource> maskSource_;
        cv::Ptr<ImageMotionEstimatorBase> motionEstimator_;
        std::shared_ptr<DeblurerBase> deblurer_;
        std::shared_ptr<InpainterBase> inpainter_;
        int radius_;
        float trimRatio_;
        bool doCorrectionForInclusion_;
        int borderMode_;

        cv::Size frameSize_;
        cv::Mat frameMask_;
        int curPos_;
        int curStabilizedPos_;
        bool doDeblurring_;
        cv::Mat preProcessedFrame_;
        bool doInpainting_;
        cv::Mat inpaintingMask_;
        cv::Mat finalFrame_;
        std::vector<cv::Mat> frames_;
        std::vector<cv::Mat> motions_; // motions_[i] is the motion from i-th to i+1-th frame
        std::vector<float> blurrinessRates_;
        std::vector<cv::Mat> stabilizedFrames_;
        std::vector<cv::Mat> stabilizedMasks_;
        std::vector<cv::Mat> stabilizationMotions_;
        clock_t processingStartTime_;

        std::shared_ptr<IMotionStabilizer> motionStabilizer_;
        std::shared_ptr<WobbleSuppressorBase> wobbleSuppressor_;
        bool mustEstTrimRatio_;

        int frameCount_;
        bool isPrePassDone_;
        bool doWobbleSuppression_;
        std::vector<cv::Mat> motions2_;
        cv::Mat suppressedFrame_;
};

#endif

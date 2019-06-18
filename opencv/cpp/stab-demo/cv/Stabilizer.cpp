#include "Stabilizer.hpp"

// Project Libraries
#include "Ring_Buffer.hpp"

// for debug purposes
#define SAVE_MOTIONS 0


StabilizerBase::StabilizerBase()
{
    setLog(std::make_shared<LogToStdout>());
    setFrameSource(std::make_shared<NullFrameSource>());
    setMotionEstimator(cv::makePtr<KeypointBasedMotionEstimator>(std::make_shared<MotionEstimatorRansacL2>()));
    setDeblurer(std::make_shared<NullDeblurer>());
    setInpainter(std::make_shared<NullInpainter>());
    setRadius(15);
    setTrimRatio(0);
    setCorrectionForInclusion(false);
    setBorderMode(cv::BORDER_REPLICATE);
    curPos_ = 0;
    doDeblurring_ = false;
    doInpainting_ = false;
    processingStartTime_ = 0;
    curStabilizedPos_ = 0;

    setMotionStabilizer(std::make_shared<GaussianMotionFilter>());
    setWobbleSuppressor(std::make_shared<NullWobbleSuppressor>());
    setEstimateTrimRatio(false);
    reset();
}


void StabilizerBase::reset()
{
    frameSize_ = cv::Size(0, 0);
    frameMask_ = cv::Mat();
    curPos_ = -1;
    curStabilizedPos_ = -1;
    doDeblurring_ = false;
    preProcessedFrame_ = cv::Mat();
    doInpainting_ = false;
    inpaintingMask_ = cv::Mat();
    frames_.clear();
    motions_.clear();
    blurrinessRates_.clear();
    stabilizedFrames_.clear();
    stabilizedMasks_.clear();
    stabilizationMotions_.clear();
    processingStartTime_ = 0;

    frameCount_ = 0;
    isPrePassDone_ = false;
    doWobbleSuppression_ = false;
    motions2_.clear();
    suppressedFrame_ = cv::Mat();
}


cv::Mat StabilizerBase::nextStabilizedFrame()
{
    // check if we've processed all frames already
    if (curStabilizedPos_ == curPos_ && curStabilizedPos_ != -1)
    {
        logProcessingTime();
        return cv::Mat();
    }

    bool processed;
    do processed = doOneIteration();
    while (processed && curStabilizedPos_ == -1);

    // check if the frame source is empty
    if (curStabilizedPos_ == -1)
    {
        logProcessingTime();
        return cv::Mat();
    }

    return postProcessFrame(at(curStabilizedPos_, stabilizedFrames_));
}


bool StabilizerBase::doOneIteration()
{
    cv::Mat frame = frameSource_->nextFrame();
    if (!frame.empty())
    {
        curPos_++;

        if (curPos_ > 0)
        {
            at(curPos_, frames_) = frame;

            if (doDeblurring_)
                at(curPos_, blurrinessRates_) = calcBlurriness(frame);

            at(curPos_ - 1, motions_) = estimateMotion();

            if (curPos_ >= radius_)
            {
                curStabilizedPos_ = curPos_ - radius_;
                stabilizeFrame();
            }
        }
        else
            setUp(frame);

        log_->print(".");
        return true;
    }

    if (curStabilizedPos_ < curPos_)
    {
        curStabilizedPos_++;
        at(curStabilizedPos_ + radius_, frames_) = at(curPos_, frames_);
        at(curStabilizedPos_ + radius_ - 1, motions_) = cv::Mat::eye(3, 3, CV_32F);
        stabilizeFrame();

        log_->print(".");
        return true;
    }

    return false;
}


void StabilizerBase::setUp(const cv::Mat &firstFrame)
{
    int cacheSize = 2*radius_ + 1;
    frames_.resize(cacheSize);
    stabilizedFrames_.resize(cacheSize);
    stabilizedMasks_.resize(cacheSize);

    for (int i = -radius_; i <= 0; ++i)
        at(i, frames_) = firstFrame;

    WobbleSuppressorBase *wobble = wobbleSuppressor_.get();
    doWobbleSuppression_ = dynamic_cast<NullWobbleSuppressor*>(wobble) == 0;
    if (doWobbleSuppression_)
    {
        wobbleSuppressor_->setFrameCount(frameCount_);
        wobbleSuppressor_->setMotions(motions_);
        wobbleSuppressor_->setMotions2(motions2_);
        wobbleSuppressor_->setStabilizationMotions(stabilizationMotions_);
    }

    InpainterBase *inpaint = inpainter_.get();
    doInpainting_ = dynamic_cast<NullInpainter*>(inpaint) == 0;
    if (doInpainting_)
    {
        inpainter_->setMotionModel(motionEstimator_->motionModel());
        inpainter_->setFrames(frames_);
        inpainter_->setMotions(motions_);
        inpainter_->setStabilizedFrames(stabilizedFrames_);
        inpainter_->setStabilizationMotions(stabilizationMotions_);
    }

    DeblurerBase *deblurer = deblurer_.get();
    doDeblurring_ = dynamic_cast<NullDeblurer*>(deblurer) == 0;
    if (doDeblurring_)
    {
        blurrinessRates_.resize(2*radius_ + 1);
        float blurriness = calcBlurriness(firstFrame);
        for (int i  = -radius_; i <= 0; ++i)
            at(i, blurrinessRates_) = blurriness;
        deblurer_->setFrames(frames_);
        deblurer_->setMotions(motions_);
        deblurer_->setBlurrinessRates(blurrinessRates_);
    }

    log_->print("processing frames");
    processingStartTime_ = clock();
}


void StabilizerBase::stabilizeFrame()
{
    cv::Mat stabilizationMotion = estimateStabilizationMotion();
    if (doCorrectionForInclusion_)
        stabilizationMotion = ensureInclusionConstraint(stabilizationMotion, frameSize_, trimRatio_);

    at(curStabilizedPos_, stabilizationMotions_) = stabilizationMotion;

    if (doDeblurring_)
    {
        at(curStabilizedPos_, frames_).copyTo(preProcessedFrame_);
        deblurer_->deblur(curStabilizedPos_, preProcessedFrame_);
    }
    else
        preProcessedFrame_ = at(curStabilizedPos_, frames_);

    // apply stabilization transformation

    if (motionEstimator_->motionModel() != MM_HOMOGRAPHY)
    {
        warpAffine( preProcessedFrame_,
                    at(curStabilizedPos_, stabilizedFrames_),
                    stabilizationMotion( cv::Rect(0, 0, 3, 2)),
                                         frameSize_,
                                         cv::INTER_LINEAR,
                                         borderMode_);
    }
    else
    {
        warpPerspective( preProcessedFrame_,
                         at( curStabilizedPos_,
                             stabilizedFrames_ ),
                         stabilizationMotion,
                         frameSize_,
                         cv::INTER_LINEAR,
                         borderMode_);
    }

    if (doInpainting_)
    {
        if (motionEstimator_->motionModel() != MM_HOMOGRAPHY)
            warpAffine(
                    frameMask_, at(curStabilizedPos_, stabilizedMasks_),
                    stabilizationMotion(cv::Rect(0,0,3,2)), frameSize_, cv::INTER_NEAREST);
        else
            warpPerspective(
                    frameMask_, at(curStabilizedPos_, stabilizedMasks_),
                    stabilizationMotion, frameSize_, cv::INTER_NEAREST);

        erode(at(curStabilizedPos_, stabilizedMasks_), at(curStabilizedPos_, stabilizedMasks_),
              cv::Mat());

        at(curStabilizedPos_, stabilizedMasks_).copyTo(inpaintingMask_);

        inpainter_->inpaint( curStabilizedPos_, at(curStabilizedPos_, stabilizedFrames_), inpaintingMask_);
    }
}


cv::Mat StabilizerBase::postProcessFrame(const cv::Mat &frame)
{
    wobbleSuppressor_->suppress(curStabilizedPos_, frame, suppressedFrame_);

    // trim frame
    int dx = static_cast<int>(floor(trimRatio_ * suppressedFrame_.cols));
    int dy = static_cast<int>(floor(trimRatio_ * suppressedFrame_.rows));
    return frame(cv::Rect(dx, dy, suppressedFrame_.cols - 2*dx, suppressedFrame_.rows - 2*dy));
}


void StabilizerBase::logProcessingTime()
{
    clock_t elapsedTime = clock() - processingStartTime_;
    log_->print("\nprocessing time: %.3f sec\n", static_cast<double>(elapsedTime) / CLOCKS_PER_SEC);
}



cv::Mat StabilizerBase::nextFrame()
{
    runPrePassIfNecessary();
    return nextStabilizedFrame();
}


#if SAVE_MOTIONS
static void saveMotions(
        int frameCount, const std::vector<Mat> &motions, const std::vector<Mat> &stabilizationMotions)
{
    std::ofstream fm("log_motions.csv");
    for (int i = 0; i < frameCount - 1; ++i)
    {
        Mat_<float> M = at(i, motions);
        fm << M(0,0) << " " << M(0,1) << " " << M(0,2) << " "
           << M(1,0) << " " << M(1,1) << " " << M(1,2) << " "
           << M(2,0) << " " << M(2,1) << " " << M(2,2) << std::endl;
    }
    std::ofstream fo("log_orig.csv");
    for (int i = 0; i < frameCount; ++i)
    {
        Mat_<float> M = getMotion(0, i, motions);
        fo << M(0,0) << " " << M(0,1) << " " << M(0,2) << " "
           << M(1,0) << " " << M(1,1) << " " << M(1,2) << " "
           << M(2,0) << " " << M(2,1) << " " << M(2,2) << std::endl;
    }
    std::ofstream fs("log_stab.csv");
    for (int i = 0; i < frameCount; ++i)
    {
        Mat_<float> M = stabilizationMotions[i] * getMotion(0, i, motions);
        fs << M(0,0) << " " << M(0,1) << " " << M(0,2) << " "
           << M(1,0) << " " << M(1,1) << " " << M(1,2) << " "
           << M(2,0) << " " << M(2,1) << " " << M(2,2) << std::endl;
    }
}
#endif


void StabilizerBase::runPrePassIfNecessary()
{
    if (!isPrePassDone_)
    {
        // check if we must do wobble suppression

        WobbleSuppressorBase *wobble = wobbleSuppressor_.get();
        doWobbleSuppression_ = dynamic_cast<NullWobbleSuppressor*>(wobble) == 0;

        // estimate motions

        clock_t startTime = clock();
        log_->print("first pass: estimating motions");

        cv::Mat prevFrame, frame;
        bool ok = true, ok2 = true;

        while (!(frame = frameSource_->nextFrame()).empty())
        {
            if (frameCount_ > 0)
            {
                if (maskSource_)
                    motionEstimator_->setFrameMask(maskSource_->nextFrame());

                motions_.push_back(motionEstimator_->estimate(prevFrame, frame, &ok));

                if (doWobbleSuppression_)
                {
                    cv::Mat M = wobbleSuppressor_->motionEstimator()->estimate(prevFrame, frame, &ok2);
                    if (ok2)
                        motions2_.push_back(M);
                    else
                        motions2_.push_back(motions_.back());
                }

                if (ok)
                {
                    if (ok2) log_->print(".");
                    else log_->print("?");
                }
                else log_->print("x");
            }
            else
            {
                frameSize_ = frame.size();
                frameMask_.create(frameSize_, CV_8U);
                frameMask_.setTo(255);
            }

            prevFrame = frame;
            frameCount_++;
        }

        clock_t elapsedTime = clock() - startTime;
        log_->print("\nmotion estimation time: %.3f sec\n",
                    static_cast<double>(elapsedTime) / CLOCKS_PER_SEC);

        // add aux. motions

        for (int i = 0; i < radius_; ++i)
            motions_.push_back(cv::Mat::eye(3, 3, CV_32F));

        // stabilize

        startTime = clock();

        stabilizationMotions_.resize(frameCount_);
        motionStabilizer_->stabilize( frameCount_,
                                      motions_,
                                      std::make_pair(0, frameCount_ - 1),
                                      &stabilizationMotions_[0]);

        elapsedTime = clock() - startTime;
        log_->print("motion stabilization time: %.3f sec\n",
                    static_cast<double>(elapsedTime) / CLOCKS_PER_SEC);

        // estimate optimal trim ratio if necessary

        if (mustEstTrimRatio_)
        {
            trimRatio_ = 0;
            for (int i = 0; i < frameCount_; ++i)
            {
                cv::Mat S = stabilizationMotions_[i];
                trimRatio_ = std::max(trimRatio_, estimateOptimalTrimRatio(S, frameSize_));
            }
            log_->print("estimated trim ratio: %f\n", static_cast<double>(trimRatio_));
        }

#if SAVE_MOTIONS
        saveMotions(frameCount_, motions_, stabilizationMotions_);
#endif

        isPrePassDone_ = true;
        frameSource_->reset();
    }
}


cv::Mat StabilizerBase::estimateMotion()
{
    return motions_[curPos_ - 1].clone();
}


cv::Mat StabilizerBase::estimateStabilizationMotion()
{
    return stabilizationMotions_[curStabilizedPos_].clone();
}


#ifndef STAB_DEMO_CV_VIDEOSTAB_FRAME_SOURCE_HPP
#define STAB_DEMO_CV_VIDEOSTAB_FRAME_SOURCE_HPP

// C++ Libraries
#include <vector>

// OpenCV Libraries
#include <opencv2/core.hpp>

class IFrameSource
{
    public:

        virtual ~IFrameSource() = default;

        virtual void reset() = 0;

        virtual cv::Mat nextFrame() = 0;
};

class NullFrameSource : public IFrameSource
{
    public:

        void reset() override
        {
        }

        cv::Mat nextFrame() override
        {
            return cv::Mat();
        }
};

class CV_EXPORTS VideoFileSource : public IFrameSource
{
public:
    VideoFileSource(const String &path, bool volatileFrame = false);

    virtual void reset() CV_OVERRIDE;
    virtual Mat nextFrame() CV_OVERRIDE;

    int width();
    int height();
    int count();
    double fps();

private:
    Ptr<IFrameSource> impl;
};

class MaskFrameSource : public IFrameSource
{
public:
    MaskFrameSource(const Ptr<IFrameSource>& source): impl(source) {};

    virtual void reset() CV_OVERRIDE { impl->reset(); }
    virtual Mat nextFrame() CV_OVERRIDE {
        Mat nextFrame = impl->nextFrame();
        maskCallback_(nextFrame);
        return nextFrame;
    }

    void setMaskCallback(std::function<void(Mat&)> MaskCallback)
    {
        maskCallback_ = std::bind(MaskCallback, std::placeholders::_1);
    };

private:
    Ptr<IFrameSource> impl;
    std::function<void(Mat&)> maskCallback_;
};

//! @}

} // namespace videostab
} // namespace cv

#endif

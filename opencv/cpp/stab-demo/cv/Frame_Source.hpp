#ifndef STAB_DEMO_CV_VIDEOSTAB_FRAME_SOURCE_HPP
#define STAB_DEMO_CV_VIDEOSTAB_FRAME_SOURCE_HPP

// C++ Libraries
#include <functional>
#include <memory>
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

class VideoFileSource : public IFrameSource
{
    public:
        VideoFileSource(const cv::String &path, bool volatileFrame = false);

        void reset() override;

        cv::Mat nextFrame() override;

        int width();

        int height();

        int count();

        double fps();

    private:
        std::shared_ptr<IFrameSource> impl;
};

class MaskFrameSource : public IFrameSource
{
    public:

        /**
         * @brief Constructor
         */
        MaskFrameSource(const std::shared_ptr<IFrameSource> &source)
          : impl(source)
        {};

        void reset()override{ impl->reset(); }

        cv::Mat nextFrame()override
        {
            cv::Mat nextFrame = impl->nextFrame();
            maskCallback_(nextFrame);
            return nextFrame;
        }

        void setMaskCallback(std::function<void(cv::Mat & )> MaskCallback)
        {
            maskCallback_ = std::bind(MaskCallback, std::placeholders::_1);
        };

    private:
        std::shared_ptr<IFrameSource> impl;
        std::function<void(cv::Mat&)> maskCallback_;
};


#endif
